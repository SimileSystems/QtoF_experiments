#ifndef TIM_WIDGET_H
#define TIM_WIDGET_H

#include <qtof/ofExternalWidget.h>

#include <ofMain.h>

class UiMessage;

class TimWidget : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
};

#endif
