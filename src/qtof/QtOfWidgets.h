/*

  QtOfWidgets
  =============

  GENERAL INFO:

    The QtOfWidgets is tightly used together with our custom QML
    item: `QtOfExternalWidget`. The `QtOfWidgets` is used to create,
    setup, update, draw etc. your custom OF based widget (it can
    actually be any object that you want to use, but we focus on OF
    here). Qt Quick may at any point destroy or recreate a QML
    item. Therefore we use this helper to do just that. What we 
    implemented with `QtOfWidgets` is a way to create and destruct
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
    between this `ref` value and your type using `qtof_widget_add();`.
    For example to map the ref value `1` to your histogram widget
    you would call:

    `qtof_widget_add(1, new QtOfWidget<DepthKitHistogramWidgetPimpl>());`

    The QML would look something like: 

    ````qml
      QtOfExternalWidget {
        ref: 1
      }
   ```

   Now, when Qt creates your `QtOfExternalWidget` item, the
   `QtOfExternalWidget` implementation will call the `qtof_widget_*()`
   functions to create, update, draw, etc.. your custom type. The
   `qtof_widget_*()` functions use a single instance of
   `QtOfWidgets`. When Qt sees the `QtOfExternalWidget` type in the
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
#include <qtof/ofExternal.h>
#include <qtof/UiMessages.h>

/* ---------------------------------------------------- */

template<class T>
class QtOfWidget;


/* ---------------------------------------------------- */
class QtOfWidgetBase {
public:
  virtual int create() = 0;
  virtual int destroy() = 0;
  virtual int setup() = 0;
  virtual int update() = 0;
  virtual int draw() = 0;
  virtual int sendUiMessage(const UiMessage& msg) = 0; /* Send a UI message from the GUI to the widget. */
  virtual int setUiMessageListener(UiMessagesListener* lis) = 0;  /* Set the message listener that receives messages from the widget. (so other direction then `sendUiMessage()`. */
};

/* ---------------------------------------------------- */

template<class T>
class QtOfWidget : public QtOfWidgetBase,
                   public UiMessagesListener {
public:
  QtOfWidget();
  int create();
  int destroy();
  int setup();
  int update();
  int draw();
  int sendUiMessage(const UiMessage& msg);
  int setUiMessageListener(UiMessagesListener* lis); /* @todo maybe rename to setWidgetUiMessageListener() ? */
  void onUiMessage(const UiMessage& m);  /* Will be called by our `UiMessages` member. `UiMessages` will make sure that the received message is safe to sure between threads. */

private:
  T* obj;
  UiMessages qt_messages;
};

/* ---------------------------------------------------- */

class QtOfWidgets {
public:
  QtOfWidgets();
  int add(int ref, QtOfWidgetBase* fac); /* QtOfWidgets takes ownership. */
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
  std::unordered_map<int, QtOfWidgetBase*> widgets;
  int num_created_widgets;
};

/* ---------------------------------------------------- */

extern QtOfWidgets qtof_widgets;

int qtof_widget_add(int ref, QtOfWidgetBase* fac);
int qtof_widget_create(int ref);
int qtof_widget_destroy(int ref);
int qtof_widget_setup(int ref);
int qtof_widget_update(int ref);
int qtof_widget_draw(int ref);
int qtof_widget_send_message(int ref, const UiMessage& msg);
int qtof_widget_set_message_listener(int ref, UiMessagesListener* lis);
int qtof_widget_get_num_registered_widgets(); /* Returns the total number of registered widgets.  */
int qtof_widget_get_num_created_widgets(); /* Returns the total number of created widgets. This is used to start/stop the renderer at the right time, see `QtOfExternalWidget.cpp`  */

int qtof_widget_is_begin();
int qtof_widget_is_end();

/* ---------------------------------------------------- */

template<class T>
QtOfWidget<T>::QtOfWidget()
  :obj(nullptr)
{
  qt_messages.setListener(this);
}

template<class T>
int QtOfWidget<T>::create() {
  
  if (nullptr != obj) {
    qFatal("QtOfWidget<T>::create() - already created the object.");
    return -1;
  }

  obj = new T();

  return 0;
}

template <class T>
int QtOfWidget<T>::destroy() {

  if (nullptr == obj) {
    qFatal("QtOfWidget<T>::destroy() - already destroyed.");
    return -1;
  }

  obj->destroy();

  delete obj;
  obj = nullptr;

  return 0;
}

template<class T>
int QtOfWidget<T>::setup() {
  
  if (nullptr == obj) {
    qFatal("QtOfWidget<T>::setup() - not created.");
    return -1;
  }

  obj->setup();
  
  return 0;
}

/*

  @todo UPDATE!!!

  This function will call the `update()` function of the widget that
  the user implemented. This can be a widget that draws a histogram
  for example. We also call `qt_messages.notify()` which will lock the
  internal buffer and then calls `QtOfWidget<T>::onUiMessage()` for
  each of the messages which have been added. We simply forward these
  `UiMessages` to the widget too in our `onUiMessage()` handler. So
  we're making an extra step here, but hiding synchronisation
  implementation of `UiMessages`. We can simply assume that we can do
  whatever we want with the `UiMessage`s that we receive in
  `onUiMessage`.

  `messages->notify()` will call `QtOfWidget<T>::onUiMessage()` and we
  forward the received message to `obj->onUiMessage()`.

 */

template<class T>
int QtOfWidget<T>::update() {
  
  if (nullptr == obj) {
    qFatal("QtOfWidget<T>::update() - not created.");
    return -1;
  }

  qt_messages.notify();
  
  obj->update();
  
  return 0;
}

template<class T>
int QtOfWidget<T>::draw() {
  
  if (nullptr == obj) {
    qFatal("QtOfWidget<T>::draw() - not created.");
    return -1;
  }

  obj->draw();
  
  return 0;
}

template<class T>
int QtOfWidget<T>::sendUiMessage(const UiMessage& msg) {
  qt_messages.addMessage(msg);
  return 0;
}

template<class T>
void QtOfWidget<T>::onUiMessage(const UiMessage& m) {

  if (nullptr == obj) {
    qFatal("Received a UiMessage but obj is nullptr.");
    return;
  }
  
  obj->onUiMessage(m);
}


template<class T>
int QtOfWidget<T>::setUiMessageListener(UiMessagesListener* lis) {

  if (nullptr == obj) {
    qFatal("Cannot set UiMessageListener because obj is nullptr.");
    return -1;
  }

  obj->setUiMessageListener(lis);

  return 0;
}

/* ---------------------------------------------------- */

inline int QtOfWidgets::update(int ref) {

  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::update() - reference not found.");
    return -1;
  }
  
  return it->second->update();
}

inline int QtOfWidgets::draw(int ref) {

  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::draw() - reference not found.");
    return -1;
  }

  return it->second->draw();
}

#endif
