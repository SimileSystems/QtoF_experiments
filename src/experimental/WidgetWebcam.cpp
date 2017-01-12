#include <qtof/ofExternal.h>
#include <experimental/WidgetWebcam.h>

void WidgetWebcam::setup() {
  video_grabber.listDevices();
  video_grabber.setup(0, 320, 240);
}

void WidgetWebcam::update() {
    video_grabber.update();
}

void WidgetWebcam::draw() {

  glDisable(GL_DEPTH_TEST);

  video_grabber.draw(getWidgetDrawX(),
                    getWidgetDrawY(),
                    getWidgetDrawWidth(),
                    getWidgetDrawHeight()
                    );

  /*
  printf("%p, %d x %d, %d = %f\n",
         this,
         getWidgetDrawWidth(),
         getWidgetDrawHeight(),
         getWidgetWidth(),
         getPixelRatio()
         );
  */
}

void WidgetWebcam::destroy() {
}

void WidgetWebcam::onExternalEvent(const ofExternalEvent& ev) {
  
  ofExternalWidget::onExternalEvent(ev);
}


