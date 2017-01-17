#ifndef WIDGET_DEPTH_KIT_PIMPL_H
#define WIDGET_DEPTH_KIT_PIMPL_H

#include <string>

class WidgetDepthKit;
class ofExternalEvent;
class UiMessage;
class UiMessagesListener;

class WidgetDepthKitPimpl {
public:
  WidgetDepthKitPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& v);
  void onUiMessage(const UiMessage& msg);
  void getJson(int what, std::string& json);
  void setUiMessageListener(UiMessagesListener* lis);

private:
  WidgetDepthKit* ctx;
};

#endif
