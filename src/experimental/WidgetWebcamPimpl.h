#ifndef DK_WIDGET_WEBCAM_PIMPL_H
#define DK_WIDGET_WEBCAM_PIMPL_H

#include <string>

class WidgetWebcam;
class UiMessage;
class UiMessageListener;
class ofExternalEvent;

class WidgetWebcamPimpl {
public:
  WidgetWebcamPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
  void setUiMessageListener(UiMessagesListener* lis);
  
private:
  WidgetWebcam* ctx;
};

#endif
