/*

  Widgets
  =============

  GENERAL INFO:

    The Widgets is tightly used together with our custom QML
    item: `QtOfExternalWidget`. The `Widgets` is used to create,
    setup, update, draw etc. your custom OF based widget (it can
    actually be any object that you want to use, but we focus on OF
    here). Qt Quick may at any point destroy or recreate a QML
    item. Therefore we use this helper to do just that. What we 
    implemented with `Widgets` is a way to create and destruct
    your types. 

    Lets say you have created a `DepthKitHistogramWidget` that you 
    want to render inside a QML based application. Because the 
    `DepthKitHistogramWidget` uses OF code you'll need to follow
    the PIMPL idiom (see `QtOfExternalWidget`) which allows us
    to completely separate your implementation from Qt.  For this
    you create a `DepthKitHistogramWidgetPimpl` type which is very
    thin wrapper around your actual `DepthKitHistogramWidget`. It
    basically wraps around the most common functions and hides
    the implementation. 

    To use and instantiate your widget you have to use the 
    `QtOfExternalWidget` QML type. This type expects that you
    define the `ref` property. This `ref` property is a integer value
    that is used to map to a C++ type. You have create the link
    between this `ref` value and your type using `widgets_add();`.
    For example to map the ref value `1` to your histogram widget
    you would call:

    `widgets_add(1, new Widget<DepthKitHistogramWidgetPimpl>());`

    The QML would look something like: 

    ````qml
      QtOfExternalWidget {
        ref: 1
      }
   ```

   Now, when Qt creates your `QtOfExternalWidget` item, the
   `QtOfExternalWidget` implementation will call the `widgets_*()`
   functions to create, update, draw, etc.. your custom type. The
   `widgets_*()` functions use a single instance of
   `Widgets`. When Qt sees the `QtOfExternalWidget` type in the
   QML file, then the `QtOfExternalWidget` will allocate your widget
   for you. We also take care of deallocation of your widget type.

  NOTES / TODO

  sendUiMessage():
    This will add a ui message to a queue which gets handed over
    to the external widget. 

 */
#ifndef QT_OF_WIDGET_H
#define QT_OF_WIDGET_H

#include <mutex>
#include <unordered_map>
#include <QDebug>
#include <qtof/Widget.h>

/* ---------------------------------------------------- */

class Widgets {
public:
  Widgets();
  int add(int ref, WidgetBase* fac); /* Widgets takes ownership. */
  int create(int ref);
  int destroy(int ref);
  int setup(int ref);
  int update(int ref);
  int draw(int ref);
  int sendUiMessage(int ref, const UiMessage& msg);
  int setUiMessageListener(int ref, UiMessagesListener* lis); /* @todo maybe rename to setWidgetUiMessageListener() */
  int getNumRegisteredWidgets(); /* Get the total number of added widgets. Is used to start and stop the underlaying (ofExternal) renderer. */
  int getNumCreatedWidgets();
  
private:
  std::unordered_map<int, WidgetBase*> widgets;
  int num_created_widgets;
};

/* ---------------------------------------------------- */

extern Widgets widgets_mananager;

int widgets_add(int ref, WidgetBase* fac);
int widgets_create(int ref);
int widgets_destroy(int ref);
int widgets_setup(int ref);
int widgets_update(int ref);
int widgets_draw(int ref);
int widgets_send_message(int ref, const UiMessage& msg);
int widgets_set_message_listener(int ref, UiMessagesListener* lis);
int widgets_get_num_registered_widgets(); /* Returns the total number of registered widgets.  */
int widgets_get_num_created_widgets(); /* Returns the total number of created widgets. This is used to start/stop the renderer at the right time, see `QtOfExternalWidget.cpp`  */

/* ---------------------------------------------------- */

inline int Widgets::update(int ref) {

  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::update() - reference not found.");
    return -1;
  }
  
  return it->second->update();
}

inline int Widgets::draw(int ref) {

  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::draw() - reference not found.");
    return -1;
  }

  return it->second->draw();
}

#endif
