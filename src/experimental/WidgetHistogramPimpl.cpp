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

void WidgetHistogramPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void WidgetHistogramPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}
