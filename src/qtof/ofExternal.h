#ifndef OF_EXTERNAL_H
#define OF_EXTERNAL_H

#include <stdint.h>

/* ------------------------------------------------------ */

#define OF_EXT_EVENT_NONE 0
#define OF_EXT_EVENT_WINDOW_RESIZED 1
#define OF_EXT_EVENT_PIXEL_RATIO_CHANGED 2
#define OF_EXT_EVENT_MOUSE_MOVE 3   /* Sets val.mouse[0] and val.mouse[1]. */
#define OF_EXT_EVENT_MOUSE_PRESS 4
#define OF_EXT_EVENT_MOUSE_RELEASE 5
#define OF_EXT_EVENT_SIZE_CHANGED 6
#define OF_EXT_EVENT_POSITION_CHANGED 7

/* ------------------------------------------------------ */

class ofExternalSettings {
public:
  ofExternalSettings();
  bool isValid();
  
public:
  int32_t win_width;
  int32_t win_height;
  float pixel_ratio;
  int32_t gl_major_version;
  int32_t gl_minor_version;
};

/* ------------------------------------------------------ */

class ofExternalEvent {
public:
  ofExternalEvent();
  
public:
  uint32_t type;
  union {
    float f;           /* Arbitrary float val. */
    int32_t xy[2];     /* x, y */
    int32_t mouse[3];  /* x, y, button. */
  } val;
};

/* ------------------------------------------------------ */

class ofExternal;

/* ------------------------------------------------------ */

int of_external_init(const ofExternalSettings& cfg);
int of_external_is_init(); /* 0 = yes, -1 = no. */
int of_external_shutdown();
int of_external_update();
int of_external_draw();
int of_external_start_render();
int of_external_finish_render();
int of_external_send_event(const ofExternalEvent& ev);

/* ------------------------------------------------------ */

extern ofExternal* of_external;

/* ------------------------------------------------------ */

#endif
