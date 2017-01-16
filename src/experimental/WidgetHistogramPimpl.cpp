#include <experimental/WidgetHistogram.h>
#include <experimental/WidgetHistogramPimpl.h>

WidgetHistogramPimpl::WidgetHistogramPimpl()
  :ctx(nullptr)
{
  ctx = new WidgetHistogram();
}

void WidgetHistogramPimpl::setup() {
  if (nullptr != ctx) {
    ctx->setup();
  }
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

void WidgetHistogramPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void WidgetHistogramPimpl::onExternalEvent(const ofExternalEvent& ev) {
  if (nullptr != ctx){
    ctx->onExternalEvent(ev);
  }
}

void WidgetHistogramPimpl::getJson(int what, std::string& json) {
}
