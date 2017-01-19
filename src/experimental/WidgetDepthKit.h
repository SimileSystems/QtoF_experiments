#ifndef WIDGET_DEPTH_KIT_H
#define WIDGET_DEPTH_KIT_H

#include <ofMain.h>
#include <qtof/ofExternalWidget.h>

class UiMessage;

class WidgetDepthKit : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);

private:
  ofVideoGrabber video_grabber;
};

#endif