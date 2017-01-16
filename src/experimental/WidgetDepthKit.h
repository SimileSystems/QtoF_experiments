#ifndef WIDGET_DEPTH_KIT_H
#define WIDGET_DEPTH_KIT_H

#include <ofMain.h>
#include <qtof/ofExternalWidget.h>

class ofExternalEvent;

class WidgetDepthKit : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& v);
  void getJson(int what, std::string& json);

private:
  ofVideoGrabber video_grabber;
};

#endif
