/*

  EXTERNAL OPENFRAMEWORKS

    This header defines an opaque way to use the openFrameworks
    library into your own code. This is basically a pimpl around
    `ofAppExternalWindow` which allows you to easily integrate OF
    without having to deal with header include conflicts (e.g. the
    annoying openGL header include issues). This is what we've been
    using to integrate OF into a Qt application. The functions defined
    here are used to setup openFrameworks with a `ofAppExternalWindow`
    and a programmable renderer. See `QtofExternal.h` which uses these
    functions to setup and use OF inside a Qt application.
  
    Before one can use the features form openFrameworks we have to
    initialize and setup openFrameworks itself. This means that we
    need to create an `application window`. For this we created the
    `ofAppExternalWindow` class. We will create and setup this when
    you call `of_external_init()`. So all you have to know is that you
    have to call `of_external_init()` to kick off openFrameworks. Of
    course there is more to it, like calling `of_external_update()`,
    `of_external_start_render()`, etc.  See `ofAppExternalWindow.h`
    for more info how this works and what functions need to be
    called. You can also have a look in `QtOfExternal.cpp` which uses
    the functions that are defined here.

 */
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
  int32_t win_width;                                       /* The width of the window at creation time. */
  int32_t win_height;                                      /* The height of the window at creation time.*/
  float pixel_ratio;                                       /* The pixel ratio of the screen; it's up to the user to handle this as OF is not hi-dpi ready at this moment. */
  int32_t gl_major_version;                                /* The major GL version for which -you- (or e.g. Qt) are setting up the context. e.g. 3 */
  int32_t gl_minor_version;                                /* The minor GL version for which -you- (or e.g. Qt) are setting up the context. e.g. 0 */
};

/* ------------------------------------------------------ */

class ofExternal;

/* ------------------------------------------------------ */

int of_external_init(const ofExternalSettings& cfg);       /* You must call this to initalize openFrameworks. */
int of_external_is_init();                                 /* Used internally to check if we're initalized. 0 = yes, -1 = no. */
int of_external_shutdown();                                /* For now a placeholder; but this can be used in the future to cleanup OF when you're closing your window.  */
int of_external_update();                                  /* Must be called regularly from the render thread from which you also called `of_external_init()` this will flush the added messages (see `of_external_send_message()`). */
int of_external_start_render();                            /* This must be called every time you want to render; this setups up the programmable renderer and correct viewport for OF. */
int of_external_finish_render();                           /* Cleans up the render state; call this when you're ready with rendering your graphics. */
int of_external_send_message(const UiMessage& msg);        /* This is used to communicate between your layer (e.g. Qt) and openFrameworks. This is used to e.g. set the new window size when it changes, mouse positions etc.. See `UiMessages.h`. */

/* ------------------------------------------------------ */

extern ofExternal* of_external;

/* ------------------------------------------------------ */

#endif
