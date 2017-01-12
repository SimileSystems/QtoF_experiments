#include <experimental/WidgetWebcam.h>
#include <experimental/WidgetWebcamPimpl.h>

WidgetWebcamPimpl::WidgetWebcamPimpl()
  :ctx(nullptr)
{
}

void WidgetWebcamPimpl::setup() {
  ctx = new WidgetWebcam();
  ctx->setup();
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

