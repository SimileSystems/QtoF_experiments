#ifndef OF_EXTERNAL_H
#define OF_EXTERNAL_H

#include <stdint.h>
#include <qtof/UiMessages.h>

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

class ofExternal;

/* ------------------------------------------------------ */

int of_external_init(const ofExternalSettings& cfg);
int of_external_is_init(); /* 0 = yes, -1 = no. */
int of_external_shutdown();
int of_external_update();
int of_external_draw();
int of_external_start_render();
int of_external_finish_render();
int of_external_send_message(const UiMessage& msg);

/* ------------------------------------------------------ */

extern ofExternal* of_external;

/* ------------------------------------------------------ */

#endif
