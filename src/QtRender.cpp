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
  printf("QtRender().\n");
  connect(this, &QQuickItem::windowChanged, this, &QtRender::onWindowChanged);
}

void QtRender::onSync() {

  printf("QtRender:sync().\n");

  if (NULL == ctx) {

    if (0 != render_alloc(&ctx)) {
      exit(EXIT_FAILURE);
    }

    connect(window(), &QQuickWindow::beforeRendering, this, &QtRender::onPaint, Qt::DirectConnection);

    /* Make sure that we at least render our scene once. */
    window()->update();
  }
}

void QtRender::onCleanup() {

  printf("QtRender:clean().\n");
  
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

  if (NULL == ctx) {
    printf("ctx is NULL cannot paint.\n");
    return;
  }

  /* Initialize opengl if needed.. */
  if (false == is_init) {
    
    if (0 != render_init(ctx)) {
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
