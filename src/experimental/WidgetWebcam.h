#ifndef WIDGET_WEBCAM_H
#define WIDGET_WEBCAM_H

#include <ofMain.h>
#include <qtof/ofExternalWidget.h>
#include <experimental/GuiBlur.h>

class ofExternalEvent;

class WidgetWebcam : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onExternalEvent(const ofExternalEvent& v);
  
private:
  ofVideoGrabber video_grabber;
  GuiBlur blur;
};

#endif
