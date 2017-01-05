#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogram.h>

void WidgetHistogram::setup() {
  img.load(ofFilePath::getCurrentExeDir() +"scene.jpg");
}

void WidgetHistogram::update() {
}

void WidgetHistogram::draw() {
  img.draw(10, 10, 640, 480);
}

void WidgetHistogram::onExternalEvent(const ofExternalEvent& ev) {
}

