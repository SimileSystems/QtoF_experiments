#include <qtof/ofExternal.h>
#include <experimental/WidgetWebcam.h>

void WidgetWebcam::setup() {

  video_grabber.listDevices();
  video_grabber.setup(0, 1280, 720);

  int w = getWidgetDrawWidth();
  int h = getWidgetDrawHeight();
  float s = 1.0f;
  if (0 != blur.init(w, h, w * s, h * s)) {
    exit(EXIT_FAILURE);
  }
  float m = getPixelRatio();
  blur.addRectangleToBlur(0, 20 * m, 300 * m, 440 * m);
}

void WidgetWebcam::update() {
    video_grabber.update();
}

void WidgetWebcam::draw() {
  glDisable(GL_DEPTH_TEST);
  blur.beginCaptureScene();
  video_grabber.draw(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());
  blur.endCaptureScene();
  blur.blur();
}

void WidgetWebcam::destroy() {
}

void WidgetWebcam::onExternalEvent(const ofExternalEvent& ev) {
  ofExternalWidget::onExternalEvent(ev);
}


