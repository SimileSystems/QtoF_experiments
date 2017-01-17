#include <stdlib.h>
#include <stdio.h>
#include <mutex>
#include <qtof/ofExternal.h>
#include <qtof/ofAppExternalWindow.h>

/* ------------------------------------------------------ */

ofExternal* of_external = nullptr;

/* ------------------------------------------------------ */

class ofExternal : public UiMessagesListener {
public:
  ofExternal();
  int init(const ofExternalSettings& cfg);
  int update();
  int draw();
  int startRender();
  int finishRender();
  int sendUiMessage(const UiMessage& msg);
  void onUiMessage(const UiMessage& msg);
  
private:
  std::shared_ptr<ofAppExternalWindow> win;
  UiMessages messages;
};

/* ------------------------------------------------------ */

ofExternalSettings::ofExternalSettings()
  :win_width(-1)
  ,win_height(-1)
  ,pixel_ratio(1.0f)
  ,gl_minor_version(0)
  ,gl_major_version(0)
{
}

bool ofExternalSettings::isValid() {

  if (win_width <= 0) {
    printf("Error: win_width is <= 0.\n");
    return false;
  }

  if (win_height <= 0) {
    printf("Error: win_height <= 0.\n");
    return false;
  }

  if (pixel_ratio <= 0.0f) {
    printf("Error: pixel_ration <= 0.0f.\n");
    return false;
  }

  if (gl_major_version > 4) {
    printf("Error: gl_major_version > 4.\n");
    return false;
  }

  return true;
}

/* ------------------------------------------------------ */

ofExternal::ofExternal()
  :win(nullptr)
{
  messages.setListener(this);
}

int ofExternal::init(const ofExternalSettings& cfg) {

  if (nullptr != win) {
    printf("Error: ofExternal::init(), app is not nullptr.\n");
    return -1;
  }

  ofGLWindowSettings gl_cfg;
  gl_cfg.width = cfg.win_width;
  gl_cfg.height = cfg.win_height;
  gl_cfg.glVersionMajor = cfg.gl_major_version;
  gl_cfg.glVersionMinor = cfg.gl_minor_version;

  win = std::shared_ptr<ofAppExternalWindow>(new ofAppExternalWindow());
  ofGetMainLoop()->addWindow(win);
  win->setup(gl_cfg);
  win->setPixelRatio(cfg.pixel_ratio);

  return 0;
}

int ofExternal::update() {

#if !defined(NDEBUG)  
  if (nullptr == win) {
    printf("Error: ofExternal::update(), win is nullptr; not initialized?\n");
    return -1;
  }
#endif

  messages.notify();

  return 0;
}

int ofExternal::startRender() {

#if !defined(NDEBUG)  
  if (nullptr == win) {
    printf("Error: ofExternal::startRender(), win is nullptr; not initialized?\n");
    return -1;
  }
#endif

  win->startRender();
  return 0;
}

int ofExternal::finishRender() {

#if !defined(NDEBUG)  
  if (nullptr == win) {
    printf("Error: ofExternal::finsihRender(), win is nullptr; not initialized?\n");
    return -1;
  }
#endif      

  win->finishRender();
  
  return 0;
}

int ofExternal::draw() {
  win->draw();
}

int ofExternal::sendUiMessage(const UiMessage& msg) {
  messages.addMessage(msg);
  return 0;
}

void ofExternal::onUiMessage(const UiMessage& msg) {

  switch (msg.type) {
    
    case UI_MSG_WINDOW_RESIZED: {
        win->setWindowSize(msg.i[0], msg.i[1]);
        break;
      }
        
      case UI_MSG_PIXEL_RATIO_CHANGED: {
        win->setPixelRatio(msg.f[0]);
        break;
      }
        
    case UI_MSG_MOUSE_MOVE: 
    case UI_MSG_MOUSE_PRESS:
    case UI_MSG_MOUSE_RELEASE: {
      break;
    }

    default: {
      printf("Warning: ofExternal::onUiMessge() - unhandled UiMessage.\n");
      break;
    }
  }
}

/* ------------------------------------------------------ */

int of_external_init(const ofExternalSettings& cfg) {

  if (0 == of_external_is_init()) {
    printf("Error: of_external_init(), already initialized.\n");
    return -1;
  }

  of_external = new ofExternal();
  of_external->init(cfg);

  return 0;
}

int of_external_is_init() {
  return (nullptr == of_external) ? -1 : 0;
}

int of_external_shutdown() {
  return 0;
}

int of_external_update() {
  
#if !defined(NDEBUG)  
  if (0 != of_external_is_init()) {
    printf("Error: of_external_update(), not initialized.\n");
    return -1;
  }
#endif

  return of_external->update();
}

/* @todo cleanup DO NOT USE - start_render() // finish_render() is what you want. */
int of_external_draw() {
  of_external->draw(); 
  return 0;
}

int of_external_send_message(const UiMessage& msg) {

#if !defined(NDEBUG)  
  if (0 != of_external_is_init()) {
    printf("Error: of_external_send_message(), not initialized.\n");
    return -1;
  }
#endif

  return of_external->sendUiMessage(msg);
}

int of_external_start_render() {

#if !defined(NDEBUG)  
  if (0 != of_external_is_init()) {
    printf("Error: of_external_start_render(), not initialized.\n");
    return -1;
  }
#endif

  of_external->startRender();

  return 0;
}

int of_external_finish_render() {

#if !defined(NDEBUG)  
  if (0 != of_external_is_init()) {
    printf("Error: of_external_finish_render(), not initialized.\n");
    return -1;
  }
#endif

  of_external->finishRender();

  return 0;
}

/* ------------------------------------------------------ */
