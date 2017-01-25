#include <experimental/Widget3d.h>
#include <qtof/UiMessages.h>

void Widget3d::Widget3d::setup() {
  
  if (false == img.load("cat.jpg")) {
    ofLogError() << "Failed to load cat.jpg from exe dir.";
    exit(EXIT_FAILURE);
  }
  
  box.set(100.0);
  perc_x = 0.0f;
  perc_y = 0.0f;

}

void Widget3d::update() {
  ofExternalWidget::notifyUiMessages();
}

void Widget3d::draw() {
  static float spin_x = 0.0f;

  ofSetColor(0, 0, 0, 100);
  ofDrawRectangle(getWidgetDrawX(), getWidgetDrawY(), getWidgetDrawWidth(), getWidgetDrawHeight());
  ofSetColor(255, 255, 255, 255);

  glEnable(GL_DEPTH_TEST);
  img.getTexture().bind();
  box.resetTransform();
  box.rotateDeg(perc_x * 360.0f, 0.0, 1.0, 0.0);
  box.rotateDeg(perc_y * 360.0f, 1.0, 0.0, 0.0);
  box.mapTexCoordsFromTexture(img.getTexture());
  box.setPosition(getWidgetDrawX() + getWidgetDrawWidth() * 0.5, getWidgetDrawY() + getWidgetDrawHeight() * 0.5, 0.0f);
  box.set(getWidgetDrawWidth() * 0.35);
  box.draw();

  ofDrawBitmapString("Widget3D", getWidgetDrawX() + 10, getWidgetDrawY() + 16);

  spin_x += 0.01;
}

void Widget3d::destroy() {
}

void Widget3d::onUiMessage(const UiMessage& msg) {
  switch (msg.type) {
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
    case UI_MSG_MOUSE_MOVE: {
      perc_x = float(msg.i[0]) / getWidgetWidth();
      perc_y = float(msg.i[1]) / getWidgetHeight();
      break;
    }
  }
}

