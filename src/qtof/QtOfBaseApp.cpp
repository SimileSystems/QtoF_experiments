#include <qtof/QtOfBaseApp.h>
#include <QPainter.h>
#include <QOpenGLFunctions.h>

QtOfBaseApp::QtOfBaseApp(QQuickItem *parent) 
    :QQuickPaintedItem(parent)
    ,ofWindow(nullptr)
    ,isQtWindowEventsConnected(false)
    ,repaintMode(RepaintMode::OnInputEvents)
    ,shouldRepaint(false)
    ,ofFboSamples(0)
    ,ofFboFormat(GL_RGBA)
    ,internalFboTexture(NULL)
    {
    connect(this, &QQuickItem::windowChanged, this, &QtOfBaseApp::onWindowChanged);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    setRenderTarget(QQuickPaintedItem::RenderTarget::InvertedYFramebufferObject);
}

QtOfBaseApp::~QtOfBaseApp() {
    deregisterEvents();
}

std::shared_ptr<ofAppExternalWindow> QtOfBaseApp::getOfWindowRef() {
    return ofWindow;
}

void QtOfBaseApp::onSync() {
    if (ofWindow == nullptr) {
        ofGLWindowSettings gl_cfg;
        gl_cfg.width = window()->width();
        gl_cfg.height = window()->height();
        gl_cfg.glVersionMajor = 3;
        gl_cfg.glVersionMinor = 3;

        ofWindow = std::shared_ptr<ofAppExternalWindow>(new ofAppExternalWindow());
        ofGetMainLoop()->addWindow(ofWindow);
        ofWindow->setup(gl_cfg);
        ofWindow->setPixelRatio(window()->devicePixelRatio());

        registerEvents();
        ofWindow->makeCurrent();
        ofGetMainLoop()->setCurrentWindow(ofWindow);
        ofWindow->events().notifySetup();

        ofLogNotice("QtOfBaseApp::onSync") << "ofAppExternalWindow created";
    }

    if (!isQtWindowEventsConnected) {
        /* 
            This connection is used to render the OF app into the internalOfFbo.
            This must be a direct connection to ensure that openGL calls from OF happen on the correct thread.
        */
        connect(window(), &QQuickWindow::beforeRendering, this, &QtOfBaseApp::onBeforeRenderDirect, Qt::ConnectionType::DirectConnection);

        // This connection is used to enforce the current repaintMode by forcing a repaint if necessary
        connect(window(), &QQuickWindow::beforeRendering, this, &QtOfBaseApp::onBeforeRender);

        connect(window(), &QQuickWindow::screenChanged, this, &QtOfBaseApp::onScreenChanged);
        
        isQtWindowEventsConnected = true;

        triggerRepaint();
        ofLogNotice("QtOfBaseApp::onSync") << "QQuickWindow events attached";
    }
}

void QtOfBaseApp::onCleanup() {
    disconnect(window());
    isQtWindowEventsConnected = false;
}

void QtOfBaseApp::onBeforeRender() {
    if (repaintMode == Always || shouldRepaint || repaintFbo) {
        triggerRepaint(repaintMode == Always || shouldRepaint);
        repaintFbo = false;
    }

    if (ofWindow != nullptr && (width() != ofWindow->getWidth() || height() != ofWindow->getHeight())) {
        ofWindow->setWindowSize(width(), height());
        ofWindow->events().notifyWindowResized(width(), height());
        triggerRepaint();
    }
}

void QtOfBaseApp::onBeforeRenderDirect() {
    if ((repaintMode == Always || shouldRepaint || updateFbo)) {
        preRender();
        render();
        postRender();
        updateFbo = false;
        repaintFbo = true;
    }
}

void QtOfBaseApp::triggerRepaint(bool force) {
    updateFbo = force;
    ((QQuickPaintedItem*)this)->update();
}

void QtOfBaseApp::preRender() {
    // Set the current window to this object's window in order for events to work properly with functions like ofMouseX() etc...
    ofGetMainLoop()->setCurrentWindow(ofWindow);
    ofWindow->startRender();
    glEnable(GL_MULTISAMPLE);
}

void QtOfBaseApp::postRender() {
    glDisable(GL_MULTISAMPLE);
    ofWindow->finishRender();
    resetOpenGlState();
}

void QtOfBaseApp::render() {
    if (!internalOfFbo.isAllocated() || internalOfFbo.getWidth() != width() || internalOfFbo.getHeight() != height()) {
        internalOfFbo.allocate(width(), height(), ofFboFormat, ofFboSamples);
    }
    internalOfFbo.begin();

    ofClear(0, 0, 0, 0);
    ofPushMatrix();
    eventsMutex.lock();
    
    ofWindow->events().notifyUpdate();
    ofWindow->events().notifyDraw();
    eventsMutex.unlock();
    ofPopMatrix();

    internalOfFbo.end();
    internalFboTexture = &internalOfFbo.getTexture();
}

void QtOfBaseApp::paint(QPainter * p) {
    if (internalFboTexture == NULL) {
        return;
    }

    p->beginNativePainting();

    preRender();
    internalFboTexture->draw(0, 0);
    postRender();

    p->endNativePainting();

    window()->update();
}

void QtOfBaseApp::resetOpenGlState() {
    window()->resetOpenGLState();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void QtOfBaseApp::onWindowChanged(QQuickWindow* win) {
    if (nullptr == win) {
        return;
    }

    connect(win, &QQuickWindow::beforeSynchronizing, this, &QtOfBaseApp::onSync, Qt::DirectConnection);
    connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtOfBaseApp::onCleanup, Qt::DirectConnection);
    connect(win, &QQuickWindow::widthChanged, this, &QtOfBaseApp::onWidthChanged);
    connect(win, &QQuickWindow::heightChanged, this, &QtOfBaseApp::onHeightChanged);
}

void QtOfBaseApp::onWidthChanged(int w) {
    triggerRepaint();
}

void QtOfBaseApp::onHeightChanged(int h) {
    triggerRepaint();
}

void QtOfBaseApp::onScreenChanged(QScreen* screen) {
    ofWindow->setPixelRatio(window()->devicePixelRatio());
    triggerRepaint();
}

void QtOfBaseApp::mousePressEvent(QMouseEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    ofWindow->events().notifyMousePressed(ev->pos().x(), ev->pos().y(), qtToOfMouseButton(ev->button()));
    triggerRepaint();
}

void QtOfBaseApp::mouseReleaseEvent(QMouseEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    ofWindow->events().notifyMouseReleased(ev->pos().x(), ev->pos().y(), qtToOfMouseButton(ev->button()));
    triggerRepaint();
}

void QtOfBaseApp::hoverEnterEvent(QHoverEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    shouldRepaint = repaintMode >= RepaintMode::OnFocused;
    ofWindow->events().notifyMouseMoved(ev->pos().x(), ev->pos().y());
    triggerRepaint();
}

void QtOfBaseApp::hoverLeaveEvent(QHoverEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    shouldRepaint = false;
    ofWindow->events().notifyMouseMoved(ev->pos().x(), ev->pos().y());
    triggerRepaint();
}

void QtOfBaseApp::hoverMoveEvent(QHoverEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    if ((int)(ev->pos().x()) != mouseX || (int)(ev->pos().y()) != mouseY) {
        ofWindow->events().notifyMouseMoved(ev->pos().x(), ev->pos().y());
        triggerRepaint();
    }
}

void QtOfBaseApp::mouseMoveEvent(QMouseEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    if (ofWindow->events().getMousePressed(OF_MOUSE_BUTTON_LEFT)) {
        ofWindow->events().notifyMouseDragged(ev->pos().x(), ev->pos().y(), OF_MOUSE_BUTTON_LEFT);
    } else if (ofWindow->events().getMousePressed(OF_MOUSE_BUTTON_MIDDLE)) {
        ofWindow->events().notifyMouseDragged(ev->pos().x(), ev->pos().y(), OF_MOUSE_BUTTON_MIDDLE);
    } else if (ofWindow->events().getMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
        ofWindow->events().notifyMouseDragged(ev->pos().x(), ev->pos().y(), OF_MOUSE_BUTTON_RIGHT);
    } else {
        ofWindow->events().notifyMouseMoved(ev->pos().x(), ev->pos().y());
    }
    triggerRepaint();
}

void QtOfBaseApp::wheelEvent(QWheelEvent *ev) {
    setFocus(true);
    forceActiveFocus();
    ofWindow->events().notifyMouseScrolled(ev->pos().x(), ev->pos().y(), ev->angleDelta().x() / 120.0, ev->angleDelta().y() / 120.0);
    triggerRepaint();
}

void QtOfBaseApp::keyPressEvent(QKeyEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    int key, keycode, scancode;
    uint32_t codepoint;
    qtToOfKey(ev, key, keycode, scancode, codepoint);

    /* 
        Note: this mutex is necessary to prevent a crash from accessing the
        openFrameworks keyboard data from both this event thread (notifyKeyPressed)
        and the render thread (calls to ofGetKeyPressed() within a widget).
        Mutexes are slow, so if this is found to be a bottleneck, an alternative solution should be sought
    */
    eventsMutex.lock();
    ofWindow->events().notifyKeyPressed(key, keycode, scancode, codepoint);
    eventsMutex.unlock();
    triggerRepaint();
}

void QtOfBaseApp::keyReleaseEvent(QKeyEvent* ev) {
    setFocus(true);
    forceActiveFocus();
    int key, keycode, scancode;
    uint32_t codepoint;
    qtToOfKey(ev, key, keycode, scancode, codepoint);

    eventsMutex.lock();
    ofWindow->events().notifyKeyReleased(key, keycode, scancode, codepoint);
    eventsMutex.unlock();
    triggerRepaint();
}

int QtOfBaseApp::qtToOfMouseButton(Qt::MouseButton button) {
    switch (button) {
    case Qt::NoButton:
        return -1;
        break;
    case Qt::LeftButton:
        return OF_MOUSE_BUTTON_LEFT;
        break;
    case Qt::RightButton:
        return OF_MOUSE_BUTTON_RIGHT;
        break;
    case Qt::MiddleButton:
        return OF_MOUSE_BUTTON_MIDDLE;
        break;
    case Qt::ForwardButton:
        return OF_MOUSE_BUTTON_5;
        break;
    case Qt::BackButton: 
        return OF_MOUSE_BUTTON_4;
        break;
    default:
        return button;
        break;
    }
}

void QtOfBaseApp::qtToOfKey(QKeyEvent * ev, int & key, int & keycode, int & scancode, uint32_t & codepoint) {
    key = codepoint = 0;
    keycode = ev->key();
    scancode = ev->nativeScanCode();
    switch (ev->key()) {
    case Qt::Key_F1:
        key = OF_KEY_F1;
        break;
    case Qt::Key_F2:
        key = OF_KEY_F2;
        break;
    case Qt::Key_F3:
        key = OF_KEY_F3;
        break;
    case Qt::Key_F4:
        key = OF_KEY_F4;
        break;
    case Qt::Key_F5:
        key = OF_KEY_F5;
        break;
    case Qt::Key_F6:
        key = OF_KEY_F6;
        break;
    case Qt::Key_F7:
        key = OF_KEY_F7;
        break;
    case Qt::Key_F8:
        key = OF_KEY_F8;
        break;
    case Qt::Key_F9:
        key = OF_KEY_F9;
        break;
    case Qt::Key_F10:
        key = OF_KEY_F10;
        break;
    case Qt::Key_F11:
        key = OF_KEY_F11;
        break;
    case Qt::Key_F12:
        key = OF_KEY_F12;
        break;
    case Qt::Key_Left:
        key = OF_KEY_LEFT;
        break;
    case Qt::Key_Right:
        key = OF_KEY_RIGHT;
        break;
    case Qt::Key_Up:
        key = OF_KEY_UP;
        break;
    case Qt::Key_Down:
        key = OF_KEY_DOWN;
        break;
    case Qt::Key_PageUp:
        key = OF_KEY_PAGE_UP;
        break;
    case Qt::Key_PageDown:
        key = OF_KEY_PAGE_DOWN;
        break;
    case Qt::Key_Home:
        key = OF_KEY_HOME;
        break;
    case Qt::Key_End:
        key = OF_KEY_END;
        break;
    case Qt::Key_Insert:
        key = OF_KEY_INSERT;
        break;
    case Qt::Key_Control:
        key = OF_KEY_CONTROL;
        break;
    case Qt::Key_Alt:
        key = OF_KEY_ALT;
        break;
    case Qt::Key_Shift:
        key = OF_KEY_SHIFT;
        break;
    case Qt::Key_Meta:
    case Qt::Key_Super_L:
        key = OF_KEY_LEFT_SUPER;
        break;
    case Qt::Key_Super_R:
        key = OF_KEY_RIGHT_SUPER;
        break;
    case Qt::Key_Return:
        key = OF_KEY_RETURN;
        break;
    case Qt::Key_Escape:
        key = OF_KEY_ESC;
        break;
    case Qt::Key_Tab:
        key = OF_KEY_TAB;
        break;
    case Qt::Key_Backspace:
        key = OF_KEY_BACKSPACE;
        break;
    case Qt::Key_Delete:
        key = OF_KEY_DEL;
        break;
    default:
        try {
            codepoint = ev->text().at(0).unicode();
            key = codepoint;
        } catch (const std::exception&) {
            // In the case that someone presses a button that is not handled by this switch,
            // ensure that it does not crash when converting to unicode. 
            ofLogError("Unable to convert keystroke to unicode character");
        }
        break;
    }
}

void QtOfBaseApp::registerEvents() {
    ofAddListener(ofWindow->events().setup, (ofBaseApp*)this, &ofBaseApp::setup, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().update, (ofBaseApp*)this, &ofBaseApp::update, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().draw, (ofBaseApp*)this, &ofBaseApp::draw, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().exit, (ofBaseApp*)this, &ofBaseApp::exit, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().keyPressed, (ofBaseApp*)this, &ofBaseApp::keyPressed, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().keyReleased, (ofBaseApp*)this, &ofBaseApp::keyReleased, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mouseMoved, (ofBaseApp*)this, &ofBaseApp::mouseMoved, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mouseDragged, (ofBaseApp*)this, &ofBaseApp::mouseDragged, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mousePressed, (ofBaseApp*)this, &ofBaseApp::mousePressed, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mouseReleased, (ofBaseApp*)this, &ofBaseApp::mouseReleased, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mouseScrolled, (ofBaseApp*)this, &ofBaseApp::mouseScrolled, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mouseEntered, (ofBaseApp*)this, &ofBaseApp::mouseEntered, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().mouseExited, (ofBaseApp*)this, &ofBaseApp::mouseExited, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().windowResized, (ofBaseApp*)this, &ofBaseApp::windowResized, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().messageEvent, (ofBaseApp*)this, &ofBaseApp::messageReceived, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().fileDragEvent, (ofBaseApp*)this, &ofBaseApp::dragged, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().touchCancelled, (ofBaseApp*)this, &ofBaseApp::touchCancelled, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().touchDoubleTap, (ofBaseApp*)this, &ofBaseApp::touchDoubleTap, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().touchDown, (ofBaseApp*)this, &ofBaseApp::touchDown, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().touchMoved, (ofBaseApp*)this, &ofBaseApp::touchMoved, OF_EVENT_ORDER_APP);
    ofAddListener(ofWindow->events().touchUp, (ofBaseApp*)this, &ofBaseApp::touchUp, OF_EVENT_ORDER_APP);
}

void QtOfBaseApp::deregisterEvents() {
    ofRemoveListener(ofWindow->events().setup, (ofBaseApp*)this, &ofBaseApp::setup, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().update, (ofBaseApp*)this, &ofBaseApp::update, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().draw, (ofBaseApp*)this, &ofBaseApp::draw, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().exit, (ofBaseApp*)this, &ofBaseApp::exit, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().keyPressed, (ofBaseApp*)this, &ofBaseApp::keyPressed, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().keyReleased, (ofBaseApp*)this, &ofBaseApp::keyReleased, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mouseMoved, (ofBaseApp*)this, &ofBaseApp::mouseMoved, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mouseDragged, (ofBaseApp*)this, &ofBaseApp::mouseDragged, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mousePressed, (ofBaseApp*)this, &ofBaseApp::mousePressed, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mouseReleased, (ofBaseApp*)this, &ofBaseApp::mouseReleased, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mouseScrolled, (ofBaseApp*)this, &ofBaseApp::mouseScrolled, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mouseEntered, (ofBaseApp*)this, &ofBaseApp::mouseEntered, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().mouseExited, (ofBaseApp*)this, &ofBaseApp::mouseExited, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().windowResized, (ofBaseApp*)this, &ofBaseApp::windowResized, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().messageEvent, (ofBaseApp*)this, &ofBaseApp::messageReceived, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().fileDragEvent, (ofBaseApp*)this, &ofBaseApp::dragged, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().touchCancelled, (ofBaseApp*)this, &ofBaseApp::touchCancelled, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().touchDoubleTap, (ofBaseApp*)this, &ofBaseApp::touchDoubleTap, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().touchDown, (ofBaseApp*)this, &ofBaseApp::touchDown, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().touchMoved, (ofBaseApp*)this, &ofBaseApp::touchMoved, OF_EVENT_ORDER_APP);
    ofRemoveListener(ofWindow->events().touchUp, (ofBaseApp*)this, &ofBaseApp::touchUp, OF_EVENT_ORDER_APP);
}
