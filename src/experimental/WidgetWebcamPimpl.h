#ifndef DK_WIDGET_WEBCAM_PIMPL_H
#define DK_WIDGET_WEBCAM_PIMPL_H

#include <string>

class WidgetWebcam;
class ofExternalEvent;

class WidgetWebcamPimpl {
public:
  WidgetWebcamPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& ev);
  void getJson(int what, std::string& json);
  
private:
  WidgetWebcam* ctx;
};

#endif
