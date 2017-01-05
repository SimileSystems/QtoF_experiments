#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogramImpl.h>

void WidgetHistogramImpl::setup() {
  img.load(ofFilePath::getCurrentExeDir() +"scene.jpg");
}

void WidgetHistogramImpl::update() {
}

void WidgetHistogramImpl::draw() {
  img.draw(10, 10, 640, 480);
}

void WidgetHistogramImpl::onExternalEvent(const ofExternalEvent& ev) {
}

