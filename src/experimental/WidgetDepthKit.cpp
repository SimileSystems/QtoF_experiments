#include <thread>
#include <qtof/ofExternal.h>
#include <experimental/WidgetDepthKit.h>

void WidgetDepthKit::setup() {
  
  video_grabber.listDevices();
  video_grabber.setDeviceID(1);
  video_grabber.setup(1, 640, 480);

  if (0 != dir_watcher.init(ofFilePath::getCurrentExeDir() + "scenes", this)) {
    ofLogError() << "Failed to init the dir watcher";
  }
}

void WidgetDepthKit::update() {
  ofExternalWidget::notifyUiMessages();
  video_grabber.update();
}

void WidgetDepthKit::draw() {
  video_grabber.draw(0, 0, ofGetWidth() * getPixelRatio(), ofGetHeight() * getPixelRatio());
}

void WidgetDepthKit::destroy() {
}

/* ----------------------------------------------------- */
/*
  Here we show how you can handle UiMessages. Of course you want to
  handle some error checking into this code when removing or creating
  the directory fails; for readability I've not added those.
 */
void WidgetDepthKit::onUiMessage(const UiMessage& msg) {

  switch (msg.type) {

    case UI_MSG_DIRECTORY_CREATE: {
      std::string new_dir = "auto-" +ofGetTimestampString();
      ofDirectory::createDirectory(dir_watcher.path +"/" +new_dir, false);
      break;
    }

    case UI_MSG_DIRECTORY_DELETE: {
      std::string del_dir = dir_watcher.path +"/" +msg.s;
      ofDirectory::removeDirectory(del_dir, true);
      break;
    }
      
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
  }
}

/* ----------------------------------------------------- */

void WidgetDepthKit::onDirectoryCreated(const std::string& path) {
  UiMessage msg;
  msg.type = UI_MSG_DIRECTORY_CREATED;
  msg.s = path;
  ofExternalWidget::addUiMessage(msg);
}

void WidgetDepthKit::onDirectoryRemoved(const std::string& path) {
  UiMessage msg;
  msg.type = UI_MSG_DIRECTORY_DELETED;
  msg.s = path;
  ofExternalWidget::addUiMessage(msg);
}

/* ----------------------------------------------------- */
