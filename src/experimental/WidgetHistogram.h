#ifndef DK_WIDGET_HISTOGRAM_H
#define DK_WIDGET_HISTOGRAM_H

class WidgetHistogramImpl;

class WidgetHistogram {
public:
  WidgetHistogram();
  void setup();
  void update();
  void draw();
  
public:
  WidgetHistogramImpl* ctx;
};

#endif
