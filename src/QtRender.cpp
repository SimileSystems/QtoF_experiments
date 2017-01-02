#include <stdio.h>
#include <stdlib.h>
#include <QQuickWindow>
#include <QtGui/QOpenGLContext>
#include <QtRender.h>

/* --------------------------------------- */

QtRender::QtRender()
  :ctx(NULL)
  ,is_init(false)
{
  connect(this, &QQuickItem::windowChanged, this, &QtRender::onWindowChanged);

}

void QtRender::onSync() {

  if (NULL == ctx) {

    connect(window(), &QQuickWindow::beforeRendering, this, &QtRender::onPaint, Qt::DirectConnection);

    if (0 != render_alloc(&ctx)) {
      exit(EXIT_FAILURE);
    }

    /* Make sure that we at least render our scene once. */
    window()->update();
  }
}

void QtRender::onCleanup() {
  
  if (NULL != ctx) {
    
    if (0 != render_shutdown(ctx)) {
      exit(EXIT_FAILURE);
    }
      
    if (0 != render_dealloc(ctx)) {
      exit(EXIT_FAILURE);
    }
  }
}

void QtRender::onPaint() {

  if (NULL == window()) {
    qFatal("window() is NULL.");
  }

  if (NULL == ctx) {
    printf("ctx is NULL cannot paint.\n");
    return;
  }

  /* Initialize opengl if needed.. */
  if (false == is_init) {

    int win_w = window()->width();
    int win_h = window()->height();
    
    if (0 != render_init(ctx, win_w, win_h)) {
      exit(EXIT_FAILURE);
    }
    
    is_init = true;
  }

  window()->resetOpenGLState();
  
  /* And update + render. */
  render_update(ctx);
  render_draw(ctx);

  window()->update();
}

/* --------------------------------------- */

void QtRender::onWindowChanged(QQuickWindow* win) {
  
  if (NULL == win) {
    printf("Warning: win is NULL. Closing the app?\n");
    return;
  }

  win->setClearBeforeRendering(false);
  
  connect(win, &QQuickWindow::beforeSynchronizing, this, &QtRender::onSync);
  connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtRender::onCleanup);
}

/* --------------------------------------- */
