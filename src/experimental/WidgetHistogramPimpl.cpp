#include <experimental/WidgetHistogram.h>
#include <experimental/WidgetHistogramPimpl.h>

WidgetHistogramPimpl::WidgetHistogramPimpl()
  :ctx(nullptr)
{
}

void WidgetHistogramPimpl::setup() {
  ctx = new WidgetHistogram();
  ctx->setup();
}

void WidgetHistogramPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void WidgetHistogramPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void WidgetHistogramPimpl::onExternalEvent(const ofExternalEvent& ev) {
  if (nullptr != ctx){
    ctx->onExternalEvent(ev);
  }
}
