#include <experimental/WidgetWebcam.h>
#include <experimental/WidgetWebcamPimpl.h>

WidgetWebcamPimpl::WidgetWebcamPimpl()
  :ctx(nullptr)
{
  ctx = new WidgetWebcam();
  printf("@todo WidgetWebcamPimpl: we need to handle create/destroy from QML.\n");
}

void WidgetWebcamPimpl::setup() {
  
  if (nullptr != ctx) {
    ctx->setup();
  }
}

void WidgetWebcamPimpl::update() {
  if (nullptr != ctx) {
    ctx->update();
  }
}

void WidgetWebcamPimpl::draw() {
  if (nullptr != ctx) {
    ctx->draw();
  }
}

void WidgetWebcamPimpl::destroy() {
  if (nullptr != ctx) {
    ctx->destroy();
  }
}

void WidgetWebcamPimpl::onUiMessage(const UiMessage& msg) {
  if (nullptr != ctx) {
    ctx->onUiMessage(msg);
  }
}

void WidgetWebcamPimpl::setUiMessageListener(UiMessagesListener* lis) {
  if (nullptr != ctx) {
    ctx->setUiMessageListener(lis);
  }
}


