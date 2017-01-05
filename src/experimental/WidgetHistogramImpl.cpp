#include <stdio.h>
#include <experimental/WidgetHistogramImpl.h>

void WidgetHistogramImpl::setup() {
  img.load(ofFilePath::getCurrentExeDir() +"scene.jpg");
}

void WidgetHistogramImpl::update() {
}

void WidgetHistogramImpl::draw() {
  img.draw(10, 10, 640, 480);
}

