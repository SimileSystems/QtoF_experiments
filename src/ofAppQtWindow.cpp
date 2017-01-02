#include <ofAppQtWindow.h>
#include <glad/glad.h>

/* ---------------------------------------------------- */

ofAppQtWindow::ofAppQtWindow()
  :curr_renderer(nullptr)
{
}

ofAppQtWindow::~ofAppQtWindow() {
}

void ofAppQtWindow::setup(const ofWindowSettings& settings) {

  if (!gladLoadGL()) {
    printf("Failed to load GL bindings.\n");
    return;
  }

  ofGLProgrammableRenderer* gl_renderer = NULL;
  gl_renderer = new ofGLProgrammableRenderer(this);
  //gl_renderer->viewport(0, 0, 640, 480, false); /* QT test.*/
  gl_renderer->setup(3,3);
  curr_renderer = shared_ptr<ofGLProgrammableRenderer>(gl_renderer);
  //curr_renderer->setupScreen();

  ofSetCurrentRenderer(curr_renderer);
  //setWindowShape(640, 480);
  //setWindowPosition(0, 0);
}

void ofAppQtWindow::update() {
}

void ofAppQtWindow::draw() {
  
  if (nullptr == curr_renderer) {
    printf("curr_renderer is NULL. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  printf("ofAppQtWindow::draw() - start render.\n");  
  curr_renderer->startRender();
  printf("ofAppQtWindow::draw() - start ender called.\n");
  //curr_renderer->setupScreen();
  core_events.notifyDraw();
  curr_renderer->finishRender();

}

/* ---------------------------------------------------- */
/*
  The functions below are used in `ofMainLoop.h` and define some of
  the features of this window type.
 */
void ofAppQtWindow::loop() {
}

bool ofAppQtWindow::doesLoop() {
  return false;
}

bool ofAppQtWindow::allowsMultiWindow() {
  return false;
}

bool ofAppQtWindow::needsPolling() {
  return false;
}

void ofAppQtWindow::pollEvents() {
}

/* ---------------------------------------------------- */
