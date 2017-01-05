#ifndef DK_WIDGET_HISTOGRAM_H
#define DK_WIDGET_HISTOGRAM_H

class WidgetHistogramImpl;
class ofExternalEvent;

class WidgetHistogram {
public:
  WidgetHistogram();
  void setup();
  void update();
  void draw();
  void onExternalEvent(const ofExternalEvent& ev);
  
public:
  WidgetHistogramImpl* ctx;
};

#endif
