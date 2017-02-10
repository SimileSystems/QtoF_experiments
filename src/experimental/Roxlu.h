#ifndef ROXLU_H
#define ROXLU_H

#include <qtof/ofExternalWidget.h>

class UiMessage;

class Roxlu : public ofExternalWidget {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
};

#endif
