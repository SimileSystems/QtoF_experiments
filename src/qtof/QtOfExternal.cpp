#include <qtof/ofExternal.h>
#include <qtof/QtOfExternal.h>

/* ---------------------------------------------------- */

QtOfExternal::QtOfExternal()
  :gl_minor_version(-1)
  ,gl_major_version(-1)
{
  connect(this, &QQuickItem::windowChanged, this, &QtOfExternal::onWindowChanged);
  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
}

/* ---------------------------------------------------- */

void QtOfExternal::onSync() {

  if (0 != of_external_is_init()) {

    ofExternalSettings cfg;
    cfg.gl_major_version = gl_major_version;
    cfg.gl_minor_version = gl_minor_version;
    cfg.win_width = window()->width();
    cfg.win_height = window()->height();
    cfg.pixel_ratio = window()->devicePixelRatio();
    
    if (0 != of_external_init(cfg)) {
      qFatal("Failed to initialize ofExternal.");
      return;
    }

    connect(window(), &QQuickWindow::beforeRendering, this, &QtOfExternal::onPaint, Qt::DirectConnection);
  }
}

void QtOfExternal::onPaint() {

  glClear(GL_COLOR_BUFFER_BIT);
  
  of_external_update();

  window()->update();
}

void QtOfExternal::onCleanup() {
}

/* --------------------------------------- */

void QtOfExternal::mousePressEvent(QMouseEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_PRESS;
  msg.i[0] = ev->x();
  msg.i[1] = ev->y();
  of_external_send_message(msg);
}

void QtOfExternal::mouseReleaseEvent(QMouseEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_RELEASE;
  msg.i[0] = ev->x();
  msg.i[1] = ev->y();
  of_external_send_message(msg);
}

/* Gets called when dragging ^.^ */
void QtOfExternal::mouseMoveEvent(QMouseEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_MOVE;
  msg.i[0] = ev->x();
  msg.i[1] = ev->y();
  of_external_send_message(msg);
}

void QtOfExternal::hoverMoveEvent(QHoverEvent* ev) {
  UiMessage msg;
  msg.type = UI_MSG_MOUSE_MOVE;
  msg.i[0] = ev->pos().x();
  msg.i[1] = ev->pos().y();
  of_external_send_message(msg);
}

void QtOfExternal::hoverEnterEvent(QHoverEvent* ev) {
  /* printf("ENTER: %d, %d\n", ev->pos().x(), ev->pos().y()); */
}

void QtOfExternal::hoverLeaveEvent(QHoverEvent* ev) {
  /* printf("LEAVE: %d, %d\n", ev->pos().x(), ev->pos().y()); */
}

/* ---------------------------------------------------- */

void QtOfExternal::onWindowChanged(QQuickWindow* win) {
  
  if (nullptr == win) {
    return;
  }

  connect(win, &QQuickWindow::beforeSynchronizing, this, &QtOfExternal::onSync, Qt::DirectConnection);
  connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtOfExternal::onCleanup, Qt::DirectConnection);
  connect(win, &QQuickWindow::widthChanged, this, &QtOfExternal::onWidthChanged);
  connect(win, &QQuickWindow::heightChanged, this, &QtOfExternal::onHeightChanged);
  connect(win, &QQuickWindow::screenChanged, this, &QtOfExternal::onScreenChanged);
}

void QtOfExternal::onWidthChanged(int w) {
  UiMessage msg;
  msg.type = UI_MSG_WINDOW_RESIZED;
  msg.i[0] = window()->width();
  msg.i[1] = window()->height();
  of_external_send_message(msg);
}

void QtOfExternal::onHeightChanged(int h) {
  UiMessage msg;
  msg.type = UI_MSG_WINDOW_RESIZED;
  msg.i[0] = window()->width();
  msg.i[1] = window()->height();
  of_external_send_message(msg);
}

void QtOfExternal::onScreenChanged(QScreen* screen) {
  UiMessage msg;
  msg.type = UI_MSG_PIXEL_RATIO_CHANGED;
  msg.f[0] = window()->devicePixelRatio();
  of_external_send_message(msg);
}

/* ---------------------------------------------------- */
