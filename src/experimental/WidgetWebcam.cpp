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

void WidgetWebcam::onUiMessage(const UiMessage& msg) {
  ofExternalWidget::onUiMessage(msg);
}

/*
void WidgetWebcam::getJson(int what, std::string& json) {

  std::vector<ofVideoDevice> devices = video_grabber.listDevices();
  std::stringstream ss;

  ss << "{\"devices\":[";
  
  for (size_t i = 0; i < devices.size(); ++i) {
    ofVideoDevice& dev = devices[i];
    ss << "{\"name\":\"" << dev.deviceName << "\"}";
    if ((i + 1) < devices.size()) {
      ss << ",";
    }
  }
  
  ss << "]}";

  json = ss.str();
}
*/
