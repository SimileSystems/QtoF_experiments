#ifndef OF_APP_QT_WINDOW_H
#define OF_APP_QT_WINDOW_H

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

  ofCoreEvents& events();
  std::shared_ptr<ofBaseRenderer>& renderer();

  static void loop();
	static bool doesLoop();
	static bool allowsMultiWindow();
	static bool needsPolling();
	static void pollEvents();

  glm::vec2 getWindowSize(); /* Used by renderer, matrix stack to set the viewport; w/o projection matrix will be invalid.. */
  glm::vec2 getScreenSize();  /* Not called */
  int getWidth(); /* Not called */
  int getHeight(); /* Not called */
  void setWindowShape(int w, int h); /* Not called. */

private:
  ofCoreEvents core_events;
  std::shared_ptr<ofBaseRenderer> curr_renderer;
  //std::shared_ptr<ofGLProgrammableRenderer> curr_renderer;
};

/* ---------------------------------------------------- */

inline ofCoreEvents& ofAppQtWindow::events() {
  return core_events;
}

inline std::shared_ptr<ofBaseRenderer>& ofAppQtWindow::renderer() {
  return curr_renderer;
}

inline glm::vec2 ofAppQtWindow::getWindowSize() {
  return glm::vec2(640 * 2, 480 * 2);
}

inline glm::vec2 ofAppQtWindow::getScreenSize() {
  printf("## getScreenSize()\n");
  return glm::vec2();
}

inline int ofAppQtWindow::getWidth() {
  printf("## GET WIDTH\n");
  return 0;
}

inline int ofAppQtWindow::getHeight() {
  printf("## GET HEGITH\n");
  return 0;
}

inline void ofAppQtWindow::setWindowShape(int w, int h) {
  printf("## setWindowShape: %d x %d\n", w, h);
}

/* ---------------------------------------------------- */



#endif
