#include <experimental/TimWidget.h>

void TimWidget::setup() {
}

void TimWidget::update() {
  ofExternalWidget::notifyUiMessages();
}

void TimWidget::draw() {
  /*
	printf("-------: %d, %d %d, %d\n",
         getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());
  */

  /* 	glDisable(GL_DEPTH_TEST); */
	ofSetColor(255, 0, 0, 255);
	ofDrawRectangle(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());

  //	ofSetColor(255, 255, 0, 255);
	//ofDrawRectangle(0, 100, 300, 500);
}

void TimWidget::destroy() {
}

void TimWidget::onUiMessage(const UiMessage& msg) {
  switch (msg.type) {
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
  }
}


