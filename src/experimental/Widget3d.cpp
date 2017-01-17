#include <experimental/Widget3d.h>
#include <qtof/UiMessages.h>

void Widget3d::Widget3d::setup() {
}

void Widget3d::update() {
  ofExternalWidget::notifyUiMessages();
}

void Widget3d::draw() {
  
  glDisable(GL_DEPTH_TEST);
  
  ofSetColor(255, 255, 0, 255);
  ofDrawRectangle(getWidgetDrawX(),
                  getWidgetDrawY(),
                  getWidgetDrawWidth(),
                  getWidgetDrawHeight()
                  );

  ofDrawCircle(100, 100, 100);

  ofSetColor(255, 255, 255, 255);
  /*
  printf("%d x %d / %d x %d\n",
         getWidgetDrawX(),
         getWidgetDrawY(),
         getWidgetDrawWidth(),
         getWidgetDrawHeight()
         );
  */

}

void Widget3d::destroy() {
}

void Widget3d::onUiMessage(const UiMessage& msg) {
  ofExternalWidget::onUiMessage(msg);
}

