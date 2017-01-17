#ifndef WIDGET_3D_H
#define WIDGET_3D_H

#include <ofMain.h>
#include <qtof/ofExternalWidget.h>

class UiMessage;

class Widget3d : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
};

#endif
