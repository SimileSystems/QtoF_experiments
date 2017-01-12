#ifndef DK_WIDGET_HISTOGRAM_PIMPL_H
#define DK_WIDGET_HISTOGRAM_PIMPL_H

class WidgetHistogram;
class ofExternalEvent;

class WidgetHistogramPimpl {
public:
  WidgetHistogramPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& ev);
  
public:
  WidgetHistogram* ctx;
};

#endif
