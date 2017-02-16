#include <experimental/WidgetDebug.h>

void WidgetDebug::setup() {
  if (false == img.load("cat.jpg")) {
    ofLogError() << "Failed to load cat.jpg from exe dir.";
    exit(EXIT_FAILURE);
  }
}

void WidgetDebug::update() {
  ofExternalWidget::notifyUiMessages();
}

void WidgetDebug::draw() {
  ofSetColor(255, 255, 255,255);
  img.draw(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());
}

void WidgetDebug::destroy() {
}

void WidgetDebug::onUiMessage(const UiMessage& msg) {
    printf("TESTING\n");
  switch (msg.type) {
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
  }
}


