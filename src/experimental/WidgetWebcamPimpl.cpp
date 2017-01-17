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

void WidgetWebcamPimpl::onExternalEvent(const ofExternalEvent& ev) {
  if (nullptr != ctx) {
    ctx->onExternalEvent(ev);
  }
}

void WidgetWebcamPimpl::onUiMessage(const UiMessage& msg) {
  printf("WidgetWebcamPimpl::onUiMessage\n");
}

void WidgetWebcamPimpl::setUiMessageListener(UiMessagesListener* lis) {
  printf("WidgetWebcamPimpl::setUiMessageListener() - @todo\n");
}

void WidgetWebcamPimpl::getJson(int what, std::string& result) {
  if (nullptr != ctx) {
    ctx->getJson(what, result);
  }
}
