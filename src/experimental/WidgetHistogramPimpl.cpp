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

void WidgetHistogramPimpl::onUiMessage(const UiMessage& msg) {
  printf("WidgetHistogramPimpl::onUiMessage\n");
}

void WidgetHistogramPimpl::getJson(int what, std::string& json) {
}

void WidgetHistogramPimpl::setUiMessageListener(UiMessagesListener* lis) {
  printf("WidgetHistogramPimpl::setUiMessageListener.\n");
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}
