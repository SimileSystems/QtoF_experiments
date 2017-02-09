#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogram.h>

void WidgetHistogram::setup() {

  if (false == img.load("dog.jpg")) {
    ofLogError() << "Failed to load cat.jpg from exe dir.";
    exit(EXIT_FAILURE);
  }
}

void WidgetHistogram::update() {
}

void WidgetHistogram::draw() {

  if (true == isMouseInsideWidget()) {
    ofSetColor(244, 244, 0, 127);
    ofDrawRectangle(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight()); // x * pix_ratio, y * pix_ratio, width * pix_ratio, height * pix_ratio);
    ofSetColor(255, 255, 255,255);
  }
  else {
    ofSetColor(255, 255, 255,255);
    img.draw(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());
  }

}

void WidgetHistogram::destroy() {
  printf("WidgetHistogram::destroy() - cleanup!\n");
}

void WidgetHistogram::onUiMessage(const UiMessage& msg) {
  ofExternalWidget::onUiMessage(msg);
}
