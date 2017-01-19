#include <experimental/WidgetDebug.h>
#include <experimental/WidgetDebugPimpl.h>

WidgetDebugPimpl::WidgetDebugPimpl() 
:ctx(nullptr)
{
  ctx = new WidgetDebug();
}

void WidgetDebugPimpl::setup() {
  if (nullptr != ctx) {
    ctx->setup();
  }
}

void WidgetDebugPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void WidgetDebugPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void WidgetDebugPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void WidgetDebugPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void WidgetDebugPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}

