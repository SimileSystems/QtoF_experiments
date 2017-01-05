#ifndef API_H
#define API_H

class RenderContext;

int render_alloc(RenderContext** ctx);
int render_dealloc(RenderContext* ctx);
int render_init(RenderContext* ctx, int windowWidth, int windowHeight, float pixelRatio);
int render_shutdown(RenderContext* ctx);
int render_update(RenderContext* ctx);
int render_draw(RenderContext* ctx);

int render_send_event(RenderContext* ctx, unsigned int event, void* data);

#endif
