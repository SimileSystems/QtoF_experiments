#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogram.h>

void WidgetHistogram::setup() {
  x = 0;
  y = 0;
  width = 640;
  height = 480;
  pix_ratio = 1.0f;
  img.load(ofFilePath::getCurrentExeDir() +"scene.jpg");
}

void WidgetHistogram::update() {
}

void WidgetHistogram::draw() {
  img.draw(x * pix_ratio, y * pix_ratio, width * pix_ratio, height * pix_ratio);
}

void WidgetHistogram::onExternalEvent(const ofExternalEvent& ev) {

  switch (ev.type) {
    case OF_EXT_EVENT_SIZE_CHANGED: {
      width = ev.val.xy[0];
      height = ev.val.xy[1];
      break;
    }
    case OF_EXT_EVENT_POSITION_CHANGED: {
      x = ev.val.xy[0];
      y = ev.val.xy[1];
      break;
    }
    case OF_EXT_EVENT_PIXEL_RATIO_CHANGED: {
      pix_ratio = ev.val.f;
      break;
    }
  }
  
}

