#include <stdio.h>
#include <qtof/ofExternal.h>
#include <experimental/WidgetHistogram.h>

void WidgetHistogram::setup() {
  img.load(ofFilePath::getCurrentExeDir() +"scene.jpg");
}

void WidgetHistogram::update() {
}

void WidgetHistogram::draw() {

  glDisable(GL_DEPTH_TEST);
  img.draw(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());

  if (true == isMouseInsideWidget()) {
    ofDrawRectangle(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight()); // x * pix_ratio, y * pix_ratio, width * pix_ratio, height * pix_ratio);
  }
}

void WidgetHistogram::destroy() {
  printf("WidgetHistogram::destroy() - cleanup!\n");
}

void WidgetHistogram::onExternalEvent(const ofExternalEvent& ev) {

  switch (ev.type) {
    
    case OF_EXT_EVENT_KEY_PRESS: {
      if (65 == ev.val.i) { /* A, a */
        ofSetColor(255, 0, 0, 127);
      }
      else if (66 == ev.val.i) { /* B, b */
        ofSetColor(0, 255, 0, 127);
      }
      else {
        ofSetColor(255, 255, 255, 255);
      }
      break;
    }
      /* 100 is defined in QML */
    case 100: {
      ofSetColor(ev.val.f * 255, 0, 0, 127);
      break;
    }
    case 101: {
      ofSetColor(0, ev.val.f * 255, 0, 127);
      break;
    }
    default: {
      ofExternalWidget::onExternalEvent(ev);
      break;
    }
  }
  
}

