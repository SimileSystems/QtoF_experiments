#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtOfFactories.h>
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
}

void QtOfExternalWidget::onPaint() {

  if (false == is_created) {
    
    if (0 != qtof_factory_create(ref)) {
      qFatal("Failed to create the QtOfExternalWidget.");
      return;
    }

    if (0 != qtof_factory_setup(ref)) {
      /* @todo destroy the created factory. */
      qFatal("Failed to setup the factory for reference: %d", ref);
      return;
    }
    
    is_created = true;
  }

  of_external_start_render();
  qtof_factory_update(ref);
  qtof_factory_draw(ref);
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

  /* @todo do we want to call this here? */
  win->setClearBeforeRendering(false);
}

/* ---------------------------------------------------- */
