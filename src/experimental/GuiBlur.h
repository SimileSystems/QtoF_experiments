/*
  Gui Blur
  =========

  GENERAL INFO:
  
    This class implements a basic vert/hori blur shader that will blur
    certain areas from a source texture. We created this class to be
    able to blur the contents underneath the panels for the Qt
    application for DepthKit. Currently we're looking into Qt to see
    if it's good enough to create a full fledged application with. Qt
    does have support to blur the contents underneath qml items, but
    not yet on Mac (at the time of writing).

    This implementation provides a Qt-independent solution to blur
    contents at certain areas.

  TODO:
    - doesn't take viewport changes into account. 
    - doesn't take hi/lo-dpi changes.

 */
#ifndef DK_GUI_BLUR_H
#define DK_GUI_BLUR_H

#include <ofMain.h>

/* ----------------------------------------------------------------- */

class GuiBlur {
public:
  GuiBlur();
  int init(int sourceWidth, int sourceHeight, int blurWidth, int blurHeight);
  void beginCaptureScene();
  void endCaptureScene();
  void blur(); 
  int shutdown();
  void addRectangleToBlur(int x, int y, int w, int h);
  
private:
  int createFbo();
  int createFboTexture(GLuint fbo, int w, int h, GLenum attachment, GLuint& tex);
  int createShaders();
  int getShaderSources(int steps, std::string& vert, std::string& hor);
  void drawBlurBoxes();

private:
  int scene_width;
  int scene_height;
  int blur_width;            /* Width of our blur output. */
  int blur_height;           /* Height of our blur output. */
  GLuint fbo_scene;
  GLuint fbo_blur;           /* We renders the given `sourceTex` (in blur()) into tex_vert with vertical blurring applied. */
  GLuint vao;
  GLuint tex_vert;           /* Contains the vertically blurred tex. */
  GLuint tex_hor;            /* Contains the horizontally blurred tex. */
  GLuint tex_scene;
  ofShader shader_vert;
  ofShader shader_hor;

public:
  std::vector<int> blur_boxes;
};

/* ----------------------------------------------------------------- */

inline void GuiBlur::addRectangleToBlur(int x, int y, int w, int h) {
  blur_boxes.push_back(x);
  blur_boxes.push_back(y);
  blur_boxes.push_back(w);
  blur_boxes.push_back(h);
}

/* ----------------------------------------------------------------- */

#endif
