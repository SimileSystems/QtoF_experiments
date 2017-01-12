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

  of_external_update();

  window()->update();
}

void QtOfExternal::onCleanup() {
  qDebug() << "@todo shutdown of external.";
}

/* --------------------------------------- */

void QtOfExternal::mousePressEvent(QMouseEvent* ev) {
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_MOUSE_PRESS;
  ext_ev.val.mouse[0] = ev->x();
  ext_ev.val.mouse[1] = ev->y();
  of_external_send_event(ext_ev);
}

void QtOfExternal::mouseReleaseEvent(QMouseEvent* ev) {
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_MOUSE_RELEASE;
  ext_ev.val.mouse[0] = ev->x();
  ext_ev.val.mouse[1] = ev->y();
  of_external_send_event(ext_ev);
}

/* Gets called when dragging ^.^ */
void QtOfExternal::mouseMoveEvent(QMouseEvent* ev) {
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_MOUSE_MOVE;
  ext_ev.val.mouse[0] = ev->x();
  ext_ev.val.mouse[1] = ev->y();
  of_external_send_event(ext_ev);
}

void QtOfExternal::hoverMoveEvent(QHoverEvent* ev) {
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_MOUSE_MOVE;
  ext_ev.val.mouse[0] = ev->pos().x();
  ext_ev.val.mouse[1] = ev->pos().y();
  of_external_send_event(ext_ev);
}

void QtOfExternal::hoverEnterEvent(QHoverEvent* ev) {
  printf("ENTER: %d, %d\n", ev->pos().x(), ev->pos().y());
}

void QtOfExternal::hoverLeaveEvent(QHoverEvent* ev) {
  printf("LEAVE: %d, %d\n", ev->pos().x(), ev->pos().y());
}

/* ---------------------------------------------------- */

void QtOfExternal::onWindowChanged(QQuickWindow* win) {
  
  if (nullptr == win) {
    qDebug() << "Not initializing the QtOfExternal; win is null.";
    return;
  }

  connect(win, &QQuickWindow::beforeSynchronizing, this, &QtOfExternal::onSync, Qt::DirectConnection);
  connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtOfExternal::onCleanup, Qt::DirectConnection);
  connect(win, &QQuickWindow::widthChanged, this, &QtOfExternal::onWidthChanged);
  connect(win, &QQuickWindow::heightChanged, this, &QtOfExternal::onHeightChanged);
  connect(win, &QQuickWindow::screenChanged, this, &QtOfExternal::onScreenChanged);

  win->setClearBeforeRendering(false);
}

void QtOfExternal::onWidthChanged(int w) {
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_WINDOW_RESIZED;
  ev.val.xy[0] = window()->width();
  ev.val.xy[1] = window()->height();
  of_external_send_event(ev);
}

void QtOfExternal::onHeightChanged(int h) {
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_WINDOW_RESIZED;
  ev.val.xy[0] = window()->width();
  ev.val.xy[1] = window()->height();
  of_external_send_event(ev);
}

void QtOfExternal::onScreenChanged(QScreen* screen) {
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_PIXEL_RATIO_CHANGED;
  ev.val.f = window()->devicePixelRatio();
  of_external_send_event(ev);
}

/* ---------------------------------------------------- */
