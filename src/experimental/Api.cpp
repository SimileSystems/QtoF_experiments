#include <stdio.h>
#include <stdlib.h>
#include <Api.h>
#include <Render.h>

using namespace dk;

/* ----------------------------------------- */

class RenderContext {
public:
  RenderContext();

public:
  Render render;
};

/* ----------------------------------------- */

RenderContext::RenderContext() {
}

/* ----------------------------------------- */

int render_alloc(RenderContext** ctx) {
  if (NULL == ctx) {
    printf("Error: ctx is NULL, cannot render_alloc().\n");
    return -1;
  }

  if (NULL != (*ctx)) {
    printf("Error: (*ctx) is not NULL, cannot render_alloc().\n");
    return -2;
  }

  *ctx = new RenderContext();

  return 0;
}

int render_dealloc(RenderContext* ctx) {
  
  if (NULL == ctx) {
    printf("Error: ctx is NULL, cannot render_dealloc().\n");
    return -1;
  }

  delete ctx;
  ctx = NULL;

  return 0;
}

int render_init(RenderContext* ctx, int winWidth, int winHeight, float pixRatio) {
  
  if (NULL == ctx) {
    printf("Error: ctx is NULL cannot render_init().\n");
    return -1;
  }

  return ctx->render.init(winWidth, winHeight, pixRatio);
}

int render_shutdown(RenderContext* ctx) {

  if (NULL == ctx) {
    printf("Error: ctx is NULL, cannot render_shutdown().\n");
    return -1;
  }

  return ctx->render.shutdown();
}

int render_update(RenderContext* ctx) {
  
  if (NULL == ctx) {
    printf("Error: ctx is NULL, cannot render_update().\n");
    return -1;
  }

  return ctx->render.update();
}

int render_draw(RenderContext* ctx) {
  
  if (NULL == ctx) {
    printf("Error: ctx is NULL, cannot render_draw().\n");
    return -1;
  }

  return ctx->render.draw();
}

/* ----------------------------------------- */

int render_send_event(RenderContext* ctx, unsigned int event, void* data) {

  if (NULL == ctx) {
    printf("Error: ctx is NULL, cannot render_send_event.\n");
    return -1;
  }

  return ctx->render.onEvent(event, data);
}

/* ----------------------------------------- */


