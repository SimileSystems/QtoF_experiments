#include <qtof/ofAppExternalWindow.h>

//std::shared_ptr<ofBaseRenderer> ofAppExternalWindow::programmable_renderer = NULL;

ofAppExternalWindow::ofAppExternalWindow()
  :pixel_ratio(1.0f)
  ,win_width(-1)
  ,win_height(-1)
  , programmable_renderer(NULL)
{
}

ofAppExternalWindow::~ofAppExternalWindow() {
  ofLogError() << "@todo we need to implement ~ofAppExternalWindow().";
}

void ofAppExternalWindow::setup(const ofWindowSettings& settings) {

  ofGLWindowSettings gl_cfg = settings;
  gl_cfg.glVersionMajor = 3;
  gl_cfg.glVersionMinor = 3;

  setup(gl_cfg);
}

void ofAppExternalWindow::setup(const ofGLWindowSettings& settings) {

  /* Validate settings. */
  if (settings.width <= 0) {
    ofLogError() << "ofWindowSettings.width is <= 0.";
    return;
  }

  if (settings.height <= 0) {
    ofLogError() << "ofWindowSettings.height is <= 0.";
    return;
  }

  if (settings.glVersionMajor < 3) {
    ofLogError() << "Make sure that the GL major version is >= 3.";
    return;
  }

  /* Load GL functions. We need this because this can be used as static lib. */
  if (0 == gladLoadGL()) {
    ofLogError() << "Failed to load the GL functions. Did you enable a GL context?";
    return;
  }

  /* Set window size that is used by the matrix stack to setup the viewport. */
  setWindowSize(settings.width, settings.height);

  /* Create the renderer. */
  if (programmable_renderer == NULL) {
    ofGLProgrammableRenderer* gl = NULL;
    gl = new ofGLProgrammableRenderer(this);
    gl->setup(settings.glVersionMajor, settings.glVersionMinor);
    programmable_renderer = std::shared_ptr<ofGLProgrammableRenderer>(gl);
  }

  /*

    We have to disable the auto background, because otherwise
    openframeworks will call `glClear(GL_COLOR_BUFFER_BIT |
    GL_DEPTH_BUFFER_BIT)` when we start / stop the renderer. In the
    case of the Qt window/GL manager, this would mean that the default
    framebuffer will be cleared by openFrameworks which may remove
    evertything that Qt has rendered.

   */

  // NOTE: This appears to no longer be the case now that each widget draws to its own FBO
  // ofSetBackgroundAuto(false);

  /*
    After some debugging issues with loading an image on Windows I
    found out that I had to call `ofInit()` because that's setting up
    the path loading features. See `ofAppRunner.cpp`
   */
  ofInit();
}

/* ---------------------------------------------------- */

void ofAppExternalWindow::update() { }
void ofAppExternalWindow::draw() { }

/* ---------------------------------------------------- */

void ofAppExternalWindow::loop() { }
bool ofAppExternalWindow::doesLoop() { return false; }
bool ofAppExternalWindow::allowsMultiWindow() { return true; }
bool ofAppExternalWindow::needsPolling() { return false; }
void ofAppExternalWindow::pollEvents() { }

/* ---------------------------------------------------- */
