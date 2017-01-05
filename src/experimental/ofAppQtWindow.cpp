#include <ofAppQtWindow.h>
#include <glad/glad.h>

/* ---------------------------------------------------- */

ofAppQtWindow::ofAppQtWindow()
  :curr_renderer(nullptr)
  ,pixel_ratio(1.0f)
  ,win_width(-1)
  ,win_height(-1)
{
}

ofAppQtWindow::~ofAppQtWindow() {
}

void ofAppQtWindow::setup(const ofWindowSettings& settings) {

  if (!gladLoadGL()) {
    printf("Failed to load GL bindings.\n");
    return;
  }

  if (settings.width <= 0) {
    printf("Error: ofWindowSettings::width <= 0.\n");
    return;
  }

  if (settings.height <= 0) {
    printf("Error: ofWindowSettings::height <= 0.\n");
    return;
  }

  setWindowShape(settings.width, settings.height);

  /* Create the renderer. */
  ofGLProgrammableRenderer* gl_renderer = NULL;
  gl_renderer = new ofGLProgrammableRenderer(this);
  gl_renderer->setup(3,3);
  curr_renderer = shared_ptr<ofGLProgrammableRenderer>(gl_renderer);

  /* It looks like we don't need to set the current renderer (!?). */
  /* ofSetCurrentRenderer(curr_renderer); */
}

void ofAppQtWindow::update() {
}

void ofAppQtWindow::draw() {
  
  if (nullptr == curr_renderer) {
    printf("curr_renderer is NULL. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  curr_renderer->startRender();
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
