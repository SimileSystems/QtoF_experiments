#include <experimental/Widget3dPimpl.h>
#include <experimental/Widget3d.h>

Widget3dPimpl::Widget3dPimpl()
  :ctx(nullptr)
{
  ctx = new Widget3d();
}

void Widget3dPimpl::setup() {
  if (nullptr != ctx) {
    ctx->setup();
  }
}

void Widget3dPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void Widget3dPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void Widget3dPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void Widget3dPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void Widget3dPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}
