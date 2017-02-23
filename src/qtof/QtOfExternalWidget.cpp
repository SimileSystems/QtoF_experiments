#include <QRunnable>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/ofExternal.h>
#include <qtof/Widgets.h>

/* ---------------------------------------------------- */

class QtOfExternalWidgetCleanupRunnable : public QRunnable {
public:
  QtOfExternalWidgetCleanupRunnable(int ref);
  void run();
  
private:
  int ref;
};

/* ---------------------------------------------------- */

QtOfExternalWidget::QtOfExternalWidget()
  :ref(-1)
  ,level(0)
  ,widget(WIDGET_TYPE_NONE)
  ,is_created(false)
{
  static int instance_counter = 0;
  instance_counter++;
  ref = instance_counter;
  
  connect(this, &QQuickItem::windowChanged, this, &QtOfExternalWidget::onWindowChanged);
  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
}

/* 

   When Qt decides to remove this QML item, we have to make sure that
   the widget also cleans up any of it's GL resources (textures,
   shaders, etc.). We provide a function `widgets_destroy_instance_with_ref()` that
   calls `destroy()` on the widget. But we have to make sure
   that this is called from the render thread when there is an GL
   context active. We're scheduling a render job for this purpose as
   described in the [QQuickItem][1] documentation.

   We also have to clear framebuffer because otherwise the contents of
   the widget may be still visible.

*/
QtOfExternalWidget::~QtOfExternalWidget() {

  if (nullptr != window()) {
    window()->setClearBeforeRendering(false); 
    window()->scheduleRenderJob(new QtOfExternalWidgetCleanupRunnable(ref), QQuickWindow::BeforeSynchronizingStage);
  }

  widget = WIDGET_TYPE_NONE;
  level = 0;
  ref = -1;
  is_created = false;
}

/*

  This function is called by the widget (e.g. the openFrameworks
  widget). This function will be called when the widget calls the
  `notify()` function of the `UiMessages` member. A widget should call
  `UiMessages::notify()` from it's `update()` function, or when the 
  widget extends `ofExternalWidget` it can call `notifyUiMessages()`.

  Here we copy all the default members of the `UiMessage` and store
  it into a QVariantMap which is then handed over to the `onUiMessage()`
  javascript function that you create in you QML. For example, you can 
  do something like:

  ````qml

     QtOfExternalWidget {
       id: depthkit
       widget: QtWidgetType.WEBCAM
       ref: 3
       width: app.width
       height: app.height
       x: 0
       y: 0
       function onUiMessage(msg) {
         console.log("onUiMessage in QML: ", msg.type, msg.i[0], msg.s);
       }
     }

  ````
  
 */
void QtOfExternalWidget::onUiMessage(const UiMessage& msg) {

  printf("Recieved a onUiMessage from a widget, type: %d\n", msg.type);
  
  QVariantMap js_msg;
  js_msg["type"] = msg.type;
  js_msg["s"] = QString(msg.s.c_str());

  QVariantList int_list;
  int_list.insert(0, msg.i[0]);
  int_list.insert(1, msg.i[1]);
  int_list.insert(2, msg.i[2]);
  int_list.insert(3, msg.i[3]);
  js_msg["i"] = int_list;

  QVariantList float_list;
  float_list.insert(0, msg.i[0]);
  float_list.insert(1, msg.i[1]);
  float_list.insert(2, msg.i[2]);
  float_list.insert(3, msg.i[3]);
  js_msg["f"] = float_list;
  
  QMetaObject::invokeMethod(this, "onUiMessage", Qt::QueuedConnection, Q_ARG(QVariant, QVariant::fromValue(js_msg)));
}

void QtOfExternalWidget::onSync() {

  if (false == is_created) {

    if (WIDGET_TYPE_NONE == widget) {
      qFatal("Cannot create widget because widget is not set, ref: %d", ref);
      return;
    }

    if (0 != widgets_create_instance_for_type_and_ref(widget, ref)) {
      qFatal("Failed to create the QtOfExternalWidget for widget: %d and ref: %d.", widget, ref);
      return;
    }

    widgets_set_message_listener_for_instance_with_ref(ref, this);

    /* Notify some info that's needed when setting up. */
    notifySize();
    notifyPixelRatio();
    notifyPosition();
    callOnSetupQmlHandler();
    
    /* Flush the messages */
    widgets_update_instance_with_ref(ref);

    if (0 != widgets_setup_instance_with_ref(ref)) {
      /* @todo destroy the created factory. */
      qFatal("Failed to setup the widgte for reference: %d", ref);
      return;
    }

    resetOpenGlState();
    
    is_created = true;

    if (0 == level) {
      connect(window(), &QQuickWindow::beforeRendering, this, &QtOfExternalWidget::onPaint, Qt::DirectConnection);
    }
    else if (1 == level) {
      connect(window(), &QQuickWindow::afterRendering, this, &QtOfExternalWidget::onPaint, Qt::DirectConnection);
    }
    else {
      qFatal("The level you provided for the QtOfExternalWidget must be either 0 or 1.");
    }

    /* 
       Listen to screen change event so we can update the pixel ratio.
       We only connect to this slot after we've initialized because
       otherwise the widget won't exist yet so there is nothing that
       can receive the message. See `QtOfExternal.h` for more info about
       the screen changed event.
     */
    connect(window(), &QQuickWindow::screenChanged, this, &QtOfExternalWidget::onScreenChanged);
  }
}

void QtOfExternalWidget::onCleanup() {
}

/*
  This is called for every `QtOfExternalWidget` that is defined in the
  QML. We call `notifyPosition()` because we need to make sure that
  each widget uses the latest position, because when we're animating
  we have to make sure that the widget draws at the right position.

  The call to `of_external_start_render()` and
  `of_external_finish_render()` will setup the openFrameworks
  programmable render for each widget. We need to do this because
  every widget needs to be able to draw on it's own; this is needed
  when e.g. drawing on different levels (see beforeRendering,
  afterRendering).

  @todo maybe we also want to update the width / height? 

 */
void QtOfExternalWidget::onPaint() {

  if (-1 == ref) {
    qWarning() << "QtOfExternalWidget::onPaint() being called but `ref` is -1. Not supposed to happen.";
    return;
  }

  notifyPosition(); 
  
  of_external_start_render();
  {
    widgets_update_instance_with_ref(ref);
    widgets_draw_instance_with_ref(ref);
  }
  of_external_finish_render();

  resetOpenGlState();
}

/* ---------------------------------------------------- */

/*

  This slot is signalled when the window has been created. At this
  point we connect our slots to the signals from the QQuickWindow. We
  are using the Qt::DirectConnection because we want our slots to be
  signalled from the render-thread; not the GUI-thread.

 */
void QtOfExternalWidget::onWindowChanged(QQuickWindow* win) {

  if (nullptr == win) {
    return;
  }

  connect(win, &QQuickWindow::beforeSynchronizing, this, &QtOfExternalWidget::onSync, Qt::DirectConnection);
  connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtOfExternalWidget::onCleanup, Qt::DirectConnection);

  win->setClearBeforeRendering(false);
}

void QtOfExternalWidget::onScreenChanged(QScreen* screen) {
  notifyPixelRatio();
}

/* ---------------------------------------------------- */

void QtOfExternalWidget::notifySize() {
  UiMessage msg;
  msg.type = UI_MSG_SIZE_CHANGED;
  msg.i[0] = width();
  msg.i[1] = height();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

/*
  We convert our local origin into that of the root item as our
  widgets use global coordinates and don't know about the QML scene
  graph (and it's hierarchy). See the documentation of `mapToItem()`,
  we pass `0` to map the position to the root item.
 */
void QtOfExternalWidget::notifyPosition() {

  QPointF pos = mapToItem(0, QPointF(0,0));
  UiMessage msg;
  msg.type = UI_MSG_POSITION_CHANGED;
  msg.i[0] = pos.x();
  msg.i[1] = pos.y();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::notifyPixelRatio() {
  UiMessage msg;
  msg.type = UI_MSG_PIXEL_RATIO_CHANGED;
  msg.f[0] = window()->devicePixelRatio();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::resetOpenGlState() {
  window()->resetOpenGLState();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

/*
  This function doesn't do much, but it's used to allow the GUI
  programmer to initialize the Widget. E.g. if it's a camera widget,
  this allows the user to select the default camera id. To use this,
  you create a `onSetup()` function in your QML `QtOfExternalWidget`
  component which will be called by us. In this function, you can
  setup your widget using
  e.g. `idname.sendUiMessage(QtUiMessage.USE_BLUE)`. We created this
  callback because our previous approach (using the
  Component.onCompleted handler) resulted in synchronisation issues
  between Windows and Mac: on Windows the onCompleted handler was
  called before the widget was actually created.

 */
void QtOfExternalWidget::callOnSetupQmlHandler() {
  QMetaObject::invokeMethod(this, "onSetup", Qt::QueuedConnection);
}

/* ---------------------------------------------------- */

/*
  @todo atm I'm not sure how to handle this situation. When I would
  remove the widget (widgets_destroy_instance_with_ref) this results
  in a crash because `onPaint` is still being called.

  You can test this by setting the parent of the QML node to null (in
  your QML create a button for example).

 */

void QtOfExternalWidget::releaseResources() {
  /*
  window()->scheduleRenderJob(new QtOfExternalWidgetCleanupRunnable(ref), QQuickWindow::AfterRenderingStage); // BeforeSynchronizingStage);
  ref = -1;
  QQuickItem::releaseResources();
  */
}

void QtOfExternalWidget::mousePressEvent(QMouseEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_PRESS;
  msg.i[0] = ev->x();
  msg.i[1] = ev->y();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::mouseReleaseEvent(QMouseEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_RELEASE;
  msg.i[0] = ev->x();
  msg.i[1] = ev->y();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::hoverEnterEvent(QHoverEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_ENTER;
  msg.i[0] = ev->pos().x();
  msg.i[1] = ev->pos().y();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::hoverLeaveEvent(QHoverEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_LEAVE;
  msg.i[0] = ev->pos().x();
  msg.i[1] = ev->pos().y();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::hoverMoveEvent(QHoverEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_MOVE;
  msg.i[0] = ev->pos().x();
  msg.i[1] = ev->pos().y();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::keyPressEvent(QKeyEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_KEY_PRESS;
  msg.i[0] = ev->key();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::keyReleaseEvent(QKeyEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_KEY_RELEASE;
  msg.i[0] = ev->key();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

/*

  In QML you can sent a UiMessage using `id.sendUiMessageString(100,
  "somestring")` where `id` is the id value that you've given to your
  `QtOfExernalWidget` item. We will convert the given data into a
  latin1 string and construct a `UiMessage` that we forward to the
  widget. So, lets say you created a histogram widget and you
  initialized it in your QML with an id `histogram`, then you can do
  `histogram.sendUiMessageString(100, "something")` and the widget
  itself (e.g. the openFramworks code) will receive the `UiMessage` in
  it's `onUiMessage()` listener function.

  This function was created to communicate between the GUI level and
  the widget level.

 */
void QtOfExternalWidget::sendUiMessageString(unsigned int msgType, const QString& str) {

  UiMessage msg;
  msg.type = msgType;
  msg.s = str.toLatin1().data();
  widgets_send_message_to_instance_with_ref(ref, msg);
}

void QtOfExternalWidget::sendUiMessage(unsigned int msgType) {
  UiMessage msg;
  msg.type = msgType;
  widgets_send_message_to_instance_with_ref(ref, msg);
}

/* ---------------------------------------------------- */

QtOfExternalWidgetCleanupRunnable::QtOfExternalWidgetCleanupRunnable(int ref)
  :ref(ref)
{
}

void QtOfExternalWidgetCleanupRunnable::run() {
  widgets_destroy_instance_with_ref(ref);
}

/* ---------------------------------------------------- */
