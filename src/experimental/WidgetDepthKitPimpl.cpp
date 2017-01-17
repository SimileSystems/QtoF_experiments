#include <experimental/WidgetDepthKitPimpl.h>
#include <experimental/WidgetDepthKit.h>

WidgetDepthKitPimpl::WidgetDepthKitPimpl()
  :ctx(nullptr)
{
  ctx = new WidgetDepthKit();
}

void WidgetDepthKitPimpl::setup() {
  if (nullptr != ctx) {
    ctx->setup();
  }
}

void WidgetDepthKitPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void WidgetDepthKitPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void WidgetDepthKitPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void WidgetDepthKitPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void WidgetDepthKitPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}
