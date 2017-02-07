#include <experimental/TimWidget.h>
#include <experimental/TimWidgetPimpl.h>

TimWidgetPimpl::TimWidgetPimpl() 
:ctx(nullptr)
{
  ctx = new TimWidget();
}

void TimWidgetPimpl::setup() {
  if (nullptr != ctx) {
    ctx->setup();
  }
}

void TimWidgetPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void TimWidgetPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void TimWidgetPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void TimWidgetPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void TimWidgetPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}

