#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtOfWidgets.h>
#include <qtof/ofExternal.h>

/* ---------------------------------------------------- */

QtOfExternalWidget::QtOfExternalWidget()
  :ref(-1)
  ,is_created(false)
{
  connect(this, &QQuickItem::windowChanged, this, &QtOfExternalWidget::onWindowChanged);
}

void QtOfExternalWidget::onSync() {
  
  if (false == is_created) {
    connect(window(), &QQuickWindow::beforeRendering, this, &QtOfExternalWidget::onPaint, Qt::DirectConnection);
    window()->update();
  }
}

void QtOfExternalWidget::onCleanup() {
  qDebug() << "CLEANUP!";
}

void QtOfExternalWidget::onPaint() {
  
  if (false == is_created) {
    
    if (0 != qtof_widget_create(ref)) {
      qFatal("Failed to create the QtOfExternalWidget.");
      return;
    }

    notifySize();
    notifyPixelRatio();
    notifyPosition();

    if (0 != qtof_widget_setup(ref)) {
      /* @todo destroy the created factory. */
      qFatal("Failed to setup the widgte for reference: %d", ref);
      return;
    }
    
    is_created = true;
  }

  of_external_start_render();
  qtof_widget_update(ref);
  qtof_widget_draw(ref);
  of_external_finish_render();

  window()->resetOpenGLState();
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
    qDebug() << "Not initializing the QtOfExternalWidget; win is null.";
    return;
  }

  connect(win, &QQuickWindow::beforeSynchronizing, this, &QtOfExternalWidget::onSync, Qt::DirectConnection);
  connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtOfExternalWidget::onCleanup, Qt::DirectConnection);
  connect(win, &QQuickWindow::screenChanged, this, &QtOfExternalWidget::onScreenChanged);

  /* @todo do we want to call this here? */
  win->setClearBeforeRendering(false);
}

/* ---------------------------------------------------- */

void QtOfExternalWidget::notifySize() {
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_SIZE_CHANGED;
  ev.val.xy[0] = width();
  ev.val.xy[1] = height();
  qtof_widget_send_event(ref, ev);
}

void QtOfExternalWidget::notifyPosition() {
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_POSITION_CHANGED;
  ev.val.xy[0] = x();
  ev.val.xy[1] = y();
  qtof_widget_send_event(ref, ev);
}

void QtOfExternalWidget::notifyPixelRatio() {
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_PIXEL_RATIO_CHANGED;
  ev.val.f = window()->devicePixelRatio();
  qtof_widget_send_event(ref, ev);
}

void QtOfExternalWidget::onScreenChanged(QScreen* screen) {
  notifyPixelRatio();
}

/* ---------------------------------------------------- */
