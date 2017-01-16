#ifndef WIDGET_DEPTH_KIT_PIMPL_H
#define WIDGET_DEPTH_KIT_PIMPL_H

#include <string>

class WidgetDepthKit;
class ofExternalEvent;

class WidgetDepthKitPimpl {
public:
  WidgetDepthKitPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& v);
  void getJson(int what, std::string& json);

private:
  WidgetDepthKit* ctx;
};

#endif
