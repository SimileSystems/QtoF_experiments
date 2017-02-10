#include <experimental/Roxlu.h>
#include <experimental/RoxluPimpl.h>

RoxluPimpl::RoxluPimpl() 
:ctx(nullptr)
{
  ctx = new Roxlu();
}

void RoxluPimpl::setup() {
  if (nullptr != ctx) {
    ctx->setup();
  }
}

void RoxluPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void RoxluPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void RoxluPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void RoxluPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void RoxluPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}

