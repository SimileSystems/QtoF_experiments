#ifndef WIDGET_HISTOGRAM_H
#define WIDGET_HISTOGRAM_H

#include <ofMain.h>

class ofExternalEvent;

class WidgetHistogram {
public:
  void setup();
  void update();
  void draw();
  void onExternalEvent(const ofExternalEvent& ev);
  
public:
  ofImage img;
  int width;
  int height;
};

#endif
