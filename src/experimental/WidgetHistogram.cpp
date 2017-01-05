#include <experimental/WidgetHistogram.h>
#include <experimental/WidgetHistogramImpl.h>

WidgetHistogram::WidgetHistogram()
  :ctx(nullptr)
{
}

void WidgetHistogram::setup() {
  ctx = new WidgetHistogramImpl();
  ctx->setup();
}

void WidgetHistogram::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void WidgetHistogram::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void WidgetHistogram::onExternalEvent(const ofExternalEvent& ev) {
  if (nullptr != ctx){
    ctx->onExternalEvent(ev);
  }
}
