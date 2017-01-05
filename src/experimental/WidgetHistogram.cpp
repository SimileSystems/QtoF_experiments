#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogram.h>

void WidgetHistogram::setup() {
  width = 640;
  height = 480;
  img.load(ofFilePath::getCurrentExeDir() +"scene.jpg");
}

void WidgetHistogram::update() {
}

void WidgetHistogram::draw() {
  img.draw(0, 0, width, height);
}

void WidgetHistogram::onExternalEvent(const ofExternalEvent& ev) {
  if (OF_EXT_EVENT_SIZE == ev.type) {
    width = ev.val.size[0];
    height = ev.val.size[1];
  }
}

