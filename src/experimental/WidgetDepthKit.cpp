#include <qtof/ofExternal.h>
#include <experimental/WidgetDepthKit.h>

void WidgetDepthKit::setup() {
  video_grabber.setup(0, 640, 480);

}

void WidgetDepthKit::update() {
  ofExternalWidget::notifyUiMessages();
  video_grabber.update();
}

void WidgetDepthKit::draw() {
  glDisable(GL_DEPTH_TEST);
  //  video_grabber.draw(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());
  video_grabber.draw(0, 0, ofGetWidth() * getPixelRatio(), ofGetHeight() * getPixelRatio()); 
  //  printf("%d x %d\n", getWidgetDrawWidth(), getWidgetDrawHeight());
}

void WidgetDepthKit::destroy() {
}

void WidgetDepthKit::onExternalEvent(const ofExternalEvent& v) {

  //printf("WidgetDepthKit::onExternalEvent() - Got external event: %d\n", v.type);
  
  if (200 == v.type) {
    printf("WidgetDepthKit::onExternalEvent() - create a new uimessage.");
    UiMessage msg;
    msg.type = 100;
    msg.i[0] = 123;
    msg.s = "{something}";
    ofExternalWidget::addUiMessage(msg);
  }
  
  ofExternalWidget::onExternalEvent(v);
}

void WidgetDepthKit::onUiMessage(const UiMessage& msg) {
  ofExternalWidget::onUiMessage(msg);
}

void WidgetDepthKit::getJson(int what, std::string& json) {
}

#if 0

  QtOfExternalWidget {
    id: webcam
    ref: 2
    x: 0
    y: 0
    width: 640
    height: 480
    /* 
       When the widget has been created, we call `getJson(0)` to 
       retrieve a list of webcams. The value `0` is defined by the 
       user for the specific widget. E.g. `0` is a list of webcams,
       `1` is a list of properties. 
     */
    Component.onCompleted: {
      var cams = JSON.parse(webcam.getJson(0));
      for (var i = 0; i < cams.devices.length; ++i) {
        webcam_model.append({"text": cams.devices[i].name})
      }
    }
  }

#endif
