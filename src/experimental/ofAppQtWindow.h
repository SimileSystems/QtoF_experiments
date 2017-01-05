/*

  Qt Window
  ==========

  This class is created to be used within a Qt application. It doesn't
  do much more then creating an instance of the
  `ofGLProgrammableRenderer` and returning the window size that is
  used in `ofMatrixStack` to setup the viewport.

  Note that this is not used as regular `ofApp*` class because 
  Qt creates the openGL context and the window, not OF (using 
  GLFW, glut, etc..).


  IMPORTANT:
  
    This is still experimental and we probably need to add implement
    other features like mouse/keyboard interaction, handling
    GL-context destroy/create cycli, hi-dpi, etc.

 */
#ifndef OF_APP_QT_WINDOW_H
#define OF_APP_QT_WINDOW_H

#include <assert.h>
#include <glad/glad.h>
#include <ofPoint.h>
#include <ofTypes.h>
#include <ofEvents.h>
#include <ofWindowSettings.h>
#include <ofAppBaseWindow.h>
#include <ofGLProgrammableRenderer.h>

/* ---------------------------------------------------- */

class ofAppQtWindow : public ofAppBaseWindow {
public:
  ofAppQtWindow();
  ~ofAppQtWindow();
  void setup(const ofWindowSettings& settings);
  void update();
  void draw();

  void startRender();
  void finishRender();

  ofCoreEvents& events();
  std::shared_ptr<ofBaseRenderer>& renderer();

  static void loop();
	static bool doesLoop();
	static bool allowsMultiWindow();
	static bool needsPolling();
	static void pollEvents();

  glm::vec2 getWindowSize();           /* Used by renderer, matrix stack to set the viewport; w/o projection matrix will be invalid.. */
  glm::vec2 getScreenSize();           /* Not called by OF classes */
  int getWidth();                      /* Not called by OF classes */
  int getHeight();                     /* Not called by OF classes */
  float getPixelRatio();

  void setPixelRatio(float ratio);
  void setWindowShape(int w, int h);

  void handleMouseMove(int x, int y);                  /* Should be called when the mouse is moved.*/
  void handleMousePress(int x, int y, int button);     /* Should be called when the mouse is pressed. */
  void handleMouseRelease(int x, int y, int button);   /* Should be called when the mouse is released. */
  
private:
  ofCoreEvents core_events;
  std::shared_ptr<ofBaseRenderer> curr_renderer;
  float pixel_ratio;
  int win_width;
  int win_height;
};

/* ---------------------------------------------------- */

inline ofCoreEvents& ofAppQtWindow::events() {
  return core_events;
}

inline std::shared_ptr<ofBaseRenderer>& ofAppQtWindow::renderer() {
  return curr_renderer;
}

inline glm::vec2 ofAppQtWindow::getWindowSize() {

  if (win_width == -1) {
    printf("Warning: ofAppQtWindow::getWindowSize(), width is -1. Use setWindowShape to the set the width.\n");
  }
  
  if (win_height == -1) {
    printf("Warning: ofAppQtWindow::getWindowSize(), height is -1. Use setWindowShape to the set the height.\n");
  }

  //printf("%d %d\n", win_width, win_height);
  //return glm::vec2(640, 480);
  return glm::vec2(win_width * pixel_ratio, win_height * pixel_ratio);
}

inline glm::vec2 ofAppQtWindow::getScreenSize() {
  return glm::vec2();
}

inline int ofAppQtWindow::getWidth() {
  assert(win_width > 0);
  return win_width;
}

inline int ofAppQtWindow::getHeight() {
  assert(win_height > 0);
  return win_height;
}

inline float ofAppQtWindow::getPixelRatio() {
  return pixel_ratio;
}

inline void ofAppQtWindow::setWindowShape(int w, int h) {
  assert(w > 0);
  assert(h > 0);
      
  win_width = w;
  win_height = h;
}

inline void ofAppQtWindow::setPixelRatio(float ratio) {

  if (ratio <= 0.0f) {
    printf("Error: ofAppQtWindow::setPixelRatio(), ratio <= 0.0f.\n");
  }
  
  pixel_ratio = ratio;
}

inline void ofAppQtWindow::startRender() {
  
  if (nullptr == curr_renderer) {
    printf("Error: curr_renderer is NULL.\n");
    return;
  }

  curr_renderer->startRender();
  curr_renderer->setupScreen();

  core_events.notifyDraw();
}

inline void ofAppQtWindow::finishRender() {

  if (nullptr == curr_renderer) {
    printf("Error: curr_renderer is NULL.\n");
    return;
  }

  curr_renderer->finishRender();
}

inline void ofAppQtWindow::handleMouseMove(int x, int y) {
  core_events.notifyMouseMoved(x, y);
}

inline void ofAppQtWindow::handleMousePress(int x, int y, int button) {
  core_events.notifyMousePressed(x, y, button);
}

inline void ofAppQtWindow::handleMouseRelease(int x, int y, int button) {
  core_events.notifyMouseReleased(x, y, button);
}

/* ---------------------------------------------------- */



#endif
