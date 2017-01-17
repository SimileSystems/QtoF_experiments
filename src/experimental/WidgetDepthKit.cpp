#include <thread>
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
  video_grabber.draw(0, 0, ofGetWidth() * getPixelRatio(), ofGetHeight() * getPixelRatio()); 
}

void WidgetDepthKit::destroy() {
}

void WidgetDepthKit::onUiMessage(const UiMessage& msg) {

  switch (msg.type) {
    
    case 1000: {

      /* 
         Here we start a thread, detach it and then sleep 
         for a while after which we send a message back 
         to the GUI layer. Here we simulate some work
         that will be done by e.g. DepthKit
      */
      std::thread t([&](void) {
          ofSleepMillis(1000);
          UiMessage result;
          result.type = 1000;
          result.s = msg.s;
          ofExternalWidget::addUiMessage(result);
        });
      
      t.detach();

      break;
    }
    case UI_MSG_MOUSE_PRESS: 
    case UI_MSG_MOUSE_MOVE: {
      printf("Mouse move: %d x %d\n", msg.i[0], msg.i[1]);
      break;
    }
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
  }
}
