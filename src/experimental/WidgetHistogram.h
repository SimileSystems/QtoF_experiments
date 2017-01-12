#ifndef WIDGET_HISTOGRAM_H
#define WIDGET_HISTOGRAM_H

#include <ofMain.h>
#include <qtof/ofExternalWidget.h>

class ofExternalEvent;

class WidgetHistogram : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& ev);
  
public:
  ofImage img;
};

#endif
