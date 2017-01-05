#include <ofMain.h>
class ofExternalEvent;

class WidgetHistogramImpl {
public:
  void setup();
  void update();
  void draw();
  void onExternalEvent(const ofExternalEvent& ev);
  
public:
  ofImage img;
};
