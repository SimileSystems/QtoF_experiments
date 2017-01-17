#ifndef DK_WIDGET_HISTOGRAM_PIMPL_H
#define DK_WIDGET_HISTOGRAM_PIMPL_H

class WidgetHistogram;
class ofExternalEvent;
class UiMessage;
class UiMessageListener;

class WidgetHistogramPimpl {
public:
  WidgetHistogramPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& ev);
  void onUiMessage(const UiMessage& msg);
  void getJson(int what, std::string& json);
  void setUiMessageListener(UiMessagesListener* lis);
  
public:
  WidgetHistogram* ctx;
};

#endif
