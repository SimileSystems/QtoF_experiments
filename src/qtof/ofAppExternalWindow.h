/*

  EXTERNAL WINDOW APPLICATION
  
    This class was created while working on the integration of
    openFrameworks inside a Qt application. This class was designed
    for a use case where openFrameworks is embedded in another
    engine as a static library where we, outside of openFrameworks,
    take care of window creation and setting up the openGL context.

    This application window class will create and use the
    `ofGLProgrammableRenderer` and therefore assumes that you're using
    a modern GL approach (> 2.x). 

    The most important thing that `ofAppExternalWindow` type will do
    is initializing the openFrameworks library and some of it's
    internal things that it needs to work (like a renderer and 
    core events object). 

  USED INTERNALLY BY OF:
  
    There are a couple of functions which are used internally by
    OF. The `getWindowSize()` for example, is use by `ofMatrixStack`
    to set the current opengl viewport. You, as a user of the
    `ofAppExternalWindow` type, are responsible for passing these
    properties that are used internally by OF (like the window size).

  MAIN LOOP: 
  
    Some functions that determine how `ofMainLoop` works are added
    statically to `ofAppExternalWindow`. Because the
    `ofAppExternalWindow` is managed externally, we do not rely on the
    returns values of these static functions. While creating this
    class we followed the `ofAppGLFWWindow` class.
    
  IMPORTANT:
  
    It's most likely that you don't create a `ofAppExternalWindow`
    directly.  Instead you will use the `of_external_*` functions that
    are defined in `ofExternal.h`. The `of_external_init()` function
    will create a `ofAppExternalWindow` for you and it will call
    `ofGetMainLoop()->addWindow(win)` to add the instance to the
    mainloop.  The global `ofGetCurrentRenderer()` calls
    `ofGetMainLoop()->getCurrentWindow()->renderer()` to retrieve the
    current renderer, which is why we have to do this. So if you want to
    create an instance of `ofAppExternalWindow` yourself, have a look
    at `of_eternal_init()`.

    See the `QtOfExternal.cpp` file where we're using these
    `of_external_*()` functions to setup and use the openFrameworks
    features using this `ofAppExternalWindow` type.

  USAGE:
  
    As described in `IMPORTANT` you most likely want to use the
    `of_external_*()` functions which provide an opaque way that you
    can use in your own window managing layer (like Qt). The
    `ofExternal.h` header contains everything that you need to use to
    initialize this `ofAppExternalWindow`. The `ofExternal` is
    basically the pimpl around `ofAppExternalWindow`. To initialize
    the `ofAppExternalWindow` object, you call `of_external_init()`
    with an `ofExternalSettings` object which describes what kind of
    openGL context you've created, your pixel ratio and the window
    size. Using these settings we will setup the
    `ofGLProgrammableRenderer`. This is done in
    `ofAppExternalWindow::setup()` which receives a `ofWindowSettings`
    object. 

    Once initialized (by calling `of_external_init()`), you want to
    notify the `ofAppExternalWindow` instance about window size
    changes, pixel ratio changes, mouse press etc. You do this by
    sending `UiMessage` objects using `of_external_send_message()`,
    which is thread safe. When you call `of_external_update()` the
    messages will be handled and the appropriate openFrameworks /
    ofAppExternalWindow functions will be called. Make sure to call
    `of_external_update()` for each frame and make sure that you call
    this from the same thread where you called `of_external_init()`
    and that the openGL context has been made current.

    Every time you want to render something, you have to call:

    ````c++
     of_external_start_render();
     // THIS IS WHERE YOU DRAW YOUR OWN CONTENT
     of_external_finish_render()

   ````

 */
#ifndef OF_APP_EXTERNAL_WINDOW_H
#define OF_APP_EXTERNAL_WINDOW_H

#include <assert.h>
#include <glad/glad.h>
#include <ofPoint.h>
#include <ofTypes.h>
#include <ofEvents.h>
#include <ofWindowSettings.h>
#include <ofAppBaseWindow.h>
#include <ofGLProgrammableRenderer.h>
#include <ofLog.h>

/* ---------------------------------------------------- */

class ofAppExternalWindow : public ofAppBaseWindow {
public:
  ofAppExternalWindow();
  ~ofAppExternalWindow();
  
  /* The base functions. */
  void setup(const ofWindowSettings& cfg);
  void setup(const ofGLWindowSettings& cfg);
  void update();
  void draw();

  /* Rendering. */
  void startRender();                                                 /* Starts the GL renderer (enable shaders). */
  void finishRender();                                                /* Cleans up the GL renderer. */

  /* Used by ofMainLoop (these are ignored, see top info) */
  static void loop();
	static bool doesLoop();
	static bool allowsMultiWindow();
	static bool needsPolling();
	static void pollEvents();

  /* Window and rendering. */
  glm::vec2 getWindowSize();                                          /* Used by renderer, matrix stack to set the viewport; w/o projection matrix will be invalid.. */
  glm::vec2 getScreenSize();                                          /* Get the current screen size. We multiply the window size by the pixel ratio (as done in the GLFW app).  */
  int getWidth();                                                     /* Get the current window height (see `setWindowSize()`). */
  int getHeight();                                                    /* Get the current window width (see `setWindowSize()`) . */
  float getPixelRatio();                                              /* Can be used to scale your visuals based on the current pixel ratio. E.g. when you're handling retina. */
  void setPixelRatio(float ratio);                                    /* Must be called by user, when the pixel ratio changes. */
  void setWindowSize(int w, int h);                                   /* Must be called by user, when the window size changes. */ 

  /* Get OF specific objects. */
  ofCoreEvents& events();                                             /* Returns a reference to the `ofCoreEvents` member; used in e.g. `ofMainLoop` to handle window specific events. */
  std::shared_ptr<ofBaseRenderer>& renderer();                        /* Returns a reference to the renderer ptr. Used in e.g. `ofAppRunner` to get the current renderer. */

private:
  ofCoreEvents core_events;                                           /* Used to dispatch global OF events like mousePress, mouseRelease, keyPress, etc.. */
  std::shared_ptr<ofBaseRenderer> programmable_renderer;              /* We're always using the programmable GL renderer. */
  float pixel_ratio;                                                  /* The last set pixel ratio; defaults to 1.0. */
  int win_width;                                                      /* The last set window width. */
  int win_height;                                                     /* The last set window height. */
};

/* ---------------------------------------------------- */

inline void ofAppExternalWindow::startRender() {
  
  if (nullptr == programmable_renderer) {
    ofLogWarning() << "Not initialized; renderer is nullptr. Did you call setup()?";
    return;
  }

  /* Bind the default shaders and setup the viewport. */
  programmable_renderer->startRender();
  programmable_renderer->setupScreen();

  core_events.notifyDraw();
}

inline void ofAppExternalWindow::finishRender() {

  if (nullptr == programmable_renderer) {
    ofLogWarning() << "Not initialized; renderer is nullptr. Did you call setup()";
    return;
  }

  programmable_renderer->finishRender();
}

/* ---------------------------------------------------- */

inline glm::vec2 ofAppExternalWindow::getWindowSize() {
  return glm::vec2(win_width * pixel_ratio, win_height * pixel_ratio);
}

inline glm::vec2 ofAppExternalWindow::getScreenSize() {
  ofLogWarning() << "@todo ofAppExternalWindow::getScreenSize()";
  return glm::vec2();
}

inline int ofAppExternalWindow::getWidth() {

  if (win_width <= 0) {
    ofLogWarning() << "win_width <= 0. Make sure to call setWindowSize().";
  }

  return win_width;
}

inline int ofAppExternalWindow::getHeight() {

  if (win_height <= 0) {
    ofLogWarning() << "win_height <= 0. Make sure to call setWindowSize().";
  }
  
  return win_height;
}

inline float ofAppExternalWindow::getPixelRatio() {
  return pixel_ratio;
}

/* ---------------------------------------------------- */

inline void ofAppExternalWindow::setWindowSize(int w, int h) {

  if (w <= 0) {
    ofLogError() << "Given window width is <= 0.";
  }

  if (h <= 0) {
    ofLogError() << "Given window height is <= 0.";
  }

  win_width = w;
  win_height = h;
}

inline void ofAppExternalWindow::setPixelRatio(float r) {

  if (r <= 0.0f) {
    ofLogError() << "Given pixel ratio <= 0.0f";
  }

  pixel_ratio = r;
}

/* ---------------------------------------------------- */

inline ofCoreEvents& ofAppExternalWindow::events() {
  return core_events;
}

inline std::shared_ptr<ofBaseRenderer>& ofAppExternalWindow::renderer() {
  
#if !defined(NDEBUG)
  if (nullptr == programmable_renderer) {
    ofLogError() << "Trying to retrieve the renderer but it's nullptr.";
  }
#endif
  
  return programmable_renderer;
}

/* ---------------------------------------------------- */

#endif
