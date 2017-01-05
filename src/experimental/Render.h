#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <graphics/ofImage.h>
#include <video/ofVideoGrabber.h>
#include <ofAppQtWindow.h>

namespace dk {

  class Render {
  public:
    Render();
    int init(int winWidth, int winHeight, float pixRatio);
    int update();
    int draw();
    int shutdown();
    int onEvent(unsigned int event, void* data); /* Handles GUI events. */
    
  private:
    GLuint prog;
    GLuint vert;
    GLuint frag;
    GLuint vao;
    ofImage img;
    ofVideoGrabber video_grabber;
    std::shared_ptr<ofAppQtWindow> win;
  };
  
} /* namespace dk */

#endif
