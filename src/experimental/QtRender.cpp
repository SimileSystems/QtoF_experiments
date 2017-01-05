#include <stdio.h>
#include <stdlib.h>
#include <QQuickWindow>
#include <QtGui/QOpenGLContext>
#include <QtRender.h>
#include <Events.h>

/* --------------------------------------- */

QtRender::QtRender()
  :ctx(NULL)
  ,is_init(false)
{
  connect(this, &QQuickItem::windowChanged, this, &QtRender::onWindowChanged);
  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
  
  //setMouseTracking(true);
  //QWidget::setMouseTracking(true);
}

void QtRender::onSync() {

  if (NULL == ctx) {

    connect(window(), &QQuickWindow::beforeRendering, this, &QtRender::onPaint, Qt::DirectConnection);

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

  /* Initialize opengl if needed.. */
  if (false == is_init) {

    int win_w = window()->width();
    int win_h = window()->height();
    float pix_ratio = window()->devicePixelRatio();

    if (0 != render_alloc(&ctx)) {
      exit(EXIT_FAILURE);
    }

    if (0 != render_init(ctx, win_w, win_h, pix_ratio)) {
      exit(EXIT_FAILURE);
    }
    
    is_init = true;
  }
  
  if (NULL == ctx) {
    printf("ctx is NULL cannot paint.\n");
    return;
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
  connect(win, &QQuickWindow::widthChanged, this, &QtRender::onWidthChanged);
  connect(win, &QQuickWindow::heightChanged, this, &QtRender::onHeightChanged);
  connect(win, &QQuickWindow::screenChanged, this, &QtRender::onScreenChanged);
}

/* What thread is calling this? */
void QtRender::onWidthChanged(int w) {
  int win_size[2] = { window()->width(), window()->height() };
  render_send_event(ctx, EVENT_WINDOW_RESIZED, win_size);
}

/* What thread is calling this? */
void QtRender::onHeightChanged(int h) {
  int win_size[2] = { window()->width(), window()->height() };
  render_send_event(ctx, EVENT_WINDOW_RESIZED, win_size);
}

/* What thread is calling this? */
void QtRender::onScreenChanged(QScreen* screen) {
  float pix_ratio = window()->devicePixelRatio();
  render_send_event(ctx, EVENT_PIXEL_RATIO_CHANGED, (void*)&pix_ratio);
}

/* --------------------------------------- */

void QtRender::mousePressEvent(QMouseEvent* ev) {
  printf("MOUSE PRESS\n");
  int mouse_data[] = { ev->x(), ev->y() };
  render_send_event(ctx, EVENT_MOUSE_PRESS, (void*)mouse_data);
}

void QtRender::mouseReleaseEvent(QMouseEvent* ev) {
  printf("MOUSE RELEASE\n");
  int button = -1;

  int mouse_data[] = { ev->x(), ev->y() };
  render_send_event(ctx, EVENT_MOUSE_RELEASE, (void*)mouse_data);
}

/* Gets called when dragging ^.^ */
void QtRender::mouseMoveEvent(QMouseEvent* ev) {
  printf("MOUSE MOVE\n");
  int mouse_data[] = { ev->x(), ev->y() };
  render_send_event(ctx, EVENT_MOUSE_MOVE, (void*)mouse_data);
}

void QtRender::hoverMoveEvent(QHoverEvent* ev) {
  printf("MOVE: (%d, %d)\n", ev->pos().x(), ev->pos().y());
  int mouse_data[] = { ev->pos().x(), ev->pos().y() };
  render_send_event(ctx, EVENT_MOUSE_MOVE, (void*)mouse_data);
}

void QtRender::hoverEnterEvent(QHoverEvent* ev) {
  printf("ENTER: %d, %d\n", ev->pos().x(), ev->pos().y());
}

void QtRender::hoverLeaveEvent(QHoverEvent* ev) {
  printf("LEAVE: %d, %d\n", ev->pos().x(), ev->pos().y());
}

