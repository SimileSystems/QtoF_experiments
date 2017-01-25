#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogram.h>

void WidgetHistogram::setup() {

  if (false == img.load("scene.jpg")) {
    ofLogError() << "Failed to load scene.jpg from exe dir.";
    exit(EXIT_FAILURE);
  }
}

void WidgetHistogram::update() {
}

void WidgetHistogram::draw() {

  glDisable(GL_DEPTH_TEST);  /* Necessary on Win */
  img.draw(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());

  if (true == isMouseInsideWidget()) {
    ofDrawRectangle(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight()); // x * pix_ratio, y * pix_ratio, width * pix_ratio, height * pix_ratio);
  }
}

void WidgetHistogram::destroy() {
  printf("WidgetHistogram::destroy() - cleanup!\n");
}

void WidgetHistogram::onUiMessage(const UiMessage& msg) {
  ofExternalWidget::onUiMessage(msg);
}
