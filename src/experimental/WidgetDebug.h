#ifndef WIDGET_DEBUG_H
#define WIDGET_DEBUG_H

#include <qtof/ofExternalWidget.h>

class UiMessage;

class WidgetDebug : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
};

#endif
