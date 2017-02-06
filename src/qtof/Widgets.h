/*

  WIDGETS
  =======

  GENERAL INFO:

    This `Widgets` tyep is tightly used together with our custom QML
    item: `QtOfExternalWidget`. The `Widgets` is used to create,
    setup, update, draw etc. your custom "OF" widget (it can actually
    be any object that you want to use, but we focus on OF here). We
    have created this `Widgets` manager to make it easy to add new
    custom widgets to a Qt application, where you take care of the
    rendering. Some things need to be taken into account: Qt Quick may
    at any point destroy or recreate a QML item. Therefore we use this
    helper to do just that. What we implemented with `Widgets` is a
    way to create and destruct your types. But we will also call
    `update()` and `draw()` on your custom type. Here we actually do
    not manage the calls to your widget; we just provide the means for
    it.  In `QtOfExternalWidget.cpp` you can see how this Widget
    manager is used. `QtOfExternaWidget` will call `update()`,
    `draw()`, `create()`, `destroy()` etc at the right time.
    
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
    `QtOfExternalWidget` QML type. This type expects that you define
    the `ref` property. This `ref` property is a integer value that is
    used to map to a intance of your widget. You have create the link
    between this `ref` value and your type using `widgets_add();`.
    For example to map the ref value `1` to your histogram widget you
    would call:

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
   `widgets_*()` functions use a single instance of `Widgets`. When Qt
   sees the `QtOfExternalWidget` type in the QML file, then the
   `QtOfExternalWidget` will allocate your widget for you. We also
   take care of deallocation of your widget type.

  USAGE: 
  
    We store the widgets in a global variable. As a user you don't
    need to be aware of this, you just have to make sure tha tyou use
    the `widgets_*()` functions to interact with widgets.

  UI MESSAGES:

    We've added some functions which can be used to handle
    sending messages towards a widget implementation but also
    to receive messages from a widget implementation. Imagine that
    you create a widget that renders e.g. a 3D cube and that you
    want the user to be able to rotate the cube when the user
    drags his cursor around in the widget area. Because we wanted
    a general widget implementation where we have no idea what 
    kind of features a widget will implemented we couldn't simply
    define functions like `onMouseMove()`, `onMousePress()` as we 
    probably wouldn't be able to define all functions that we 
    needed. To overcome this, we created one fuction `sendUiMessage()`
    that can be used to notify the widget about anything that
    happens in the GUI. This can be mouse events, keyboard events,
    window events or a custom event like "create a directory",
    "give me a list of directories" etc. 

    Though it's totally up to your implementation how you handle the
    messages that we sent. You have to be aware that messages can be
    sent from different threads. In our case (see Widget.h) we're
    using the `UiMessages` type which is a thread safe queue. But
    let's say you want to create your own `Widget` type make sure
    to handle messages correctly and deal with threading. 
    
    Have a look at `QtOfExternalWidget.cpp` for a use case where we're
    sending event messages that are generated by Qt (like postion
    changed, pixel ratio changed, etc), using `UiMessages`. When you
    call `widgets_send_message()` they will be added to a queue (in
    case of our implemenation of course). When `widgets_update()` is
    called, our implemtation will make sure that any of the added
    messages will be flushed and handed over to the widget.

    Another important feature we needed was a way to receive messages
    from a widget. For example we may want a widget to create a
    directory and notify us when the directory has been
    created. Again, because its totally up to the widget
    implementation how to deal with messages, we only provide a
    function `widgets_set_message_listener()` which calls the
    corresponding `setUiMessageListener()` of the `WidgetFactoryBase`
    interface. In our case we're again using a `UiMessages` object for
    sending messages from a widget back to the gui.
    

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
  int addFactoryForType(int type, WidgetFactoryBase* factory);                                 /* Add a factory object for the given widget type. */
  int createInstanceForTypeAndRef(int type, int ref);                                          /* This will create a new instance of the widget using the factory for the given type. */
  int destroyInstanceWithRef(int ref);                                                         /* This will call `destroy()` on your widget and allows you to cleanup anything you created in `create()`. Can be called multiple times with Qt (see documentation in this header). */
  int setupInstanceWithRef(int ref);                                                           /* This will call `setup()` on your widget. */
  int updateInstanceWithRef(int ref);                                                          /* This will call `update()` on your widget. */
  int drawInstanceWithRef(int ref);                                                            /* This will call `draw()` on your widget. */
  int sendUiMessageToInstanceWithRef(int ref, const UiMessage& msg);                           /* Thread safe way to send messages to your widget from e.g. the Qt Gui thread. The messages are flushed when you call `update()`. */
  int setUiMessageListenerForInstanceWithRef(int ref, UiMessagesListener* lis);                /* Set the listener that will/can receive messages back from your own widget. See `UI MESSAGES` above.  */
  int getNumRegisteredWidgets();                                                               /* Get the number of registered widgets (which have be added using `add()`. */                 
  int getNumCreatedWidgets();                                                                  /* Get the total number of created widgets. Is used to start and stop the underlaying (ofExternal) renderer. */
  
private:
  std::unordered_map<int, WidgetFactoryBase*> factories;
  std::unordered_map<int, WidgetInstanceBase*> instances;
};

/* ---------------------------------------------------- */

extern Widgets widgets_mananager;

int widgets_add_factory_for_type(int type, WidgetFactoryBase* factory);                        /* Add a factory object for the given widget type. See `WidgetTypes.h` where you can define the widget types. */
int widgets_create_instance_for_type_and_ref(int type, int ref);                               /* Calls `createWidgetInstance()` on the factory for the given reference. */
int widgets_destroy_instance_with_ref(int ref);                                                /* Calls `destroy()` on the widget instance for the given reference. */
int widgets_setup_instance_with_ref(int ref);                                                  /* Calls `setup()` on the widget for the given reference. */
int widgets_update_instance_with_ref(int ref);                                                 /* Calls `update()` on the widget for the given reference, and also makes sure the added `UiMessages` are flushed. */
int widgets_draw_instance_with_ref(int ref);                                                   /* Calls `draw()` on the widget for the given reference. */
int widgets_send_message_to_instance_with_ref(int ref, const UiMessage& msg);                  /* Send a message to the widget for the given reference. Make sure to call `widgets_update()` to flush the message queue. */
int widgets_set_message_listener_for_instance_with_ref(int ref, UiMessagesListener* lis);      /* Set the listener for message -FROM- the widget. */
int widgets_get_num_registered_widgets();                                                      /* Returns the total number of registered widgets.  */
int widgets_get_num_created_widgets();                                                         /* Returns the total number of created widgets. This is used to start/stop the renderer at the right time, see `QtOfExternalWidget.cpp`  */

/* ---------------------------------------------------- */

inline int Widgets::updateInstanceWithRef(int ref) {

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::update() - no instance found for the reference: %d", ref);
    return -1;
  }

  it->second->update();

  return 0;
}

inline int Widgets::drawInstanceWithRef(int ref) {

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::draw() - no instance found for the reference: %d", ref);
    return -1;
  }

  it->second->draw();
  
  return 0;
}

/* ---------------------------------------------------- */

#endif
