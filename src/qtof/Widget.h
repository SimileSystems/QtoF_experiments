/*

  WIDGET
  ======

  GENERAL INFO:
  
    Here we define a interface for a widget. We created an widget
    interface for the use with Qt and openFrameworks.  Though a widget
    can be anything that wants to render something in a custom
    way. See `Widgets.h` for more information.

 */
#ifndef WIDGET_H
#define WIDGET_H

#include <qtof/UiMessages.h>

/* ---------------------------------------------------- */

template<class T>
class Widget;

/* ---------------------------------------------------- */

class WidgetBase {
public:
  virtual int create() = 0;
  virtual int destroy() = 0;
  virtual int setup() = 0;
  virtual int update() = 0;
  virtual int draw() = 0;
  virtual int sendUiMessage(const UiMessage& msg) = 0;                /* Send a UI message from the GUI to the widget. */
  virtual int setUiMessageListener(UiMessagesListener* lis) = 0;      /* Set the message listener that receives messages from the widget. (so other direction then `sendUiMessage()`. */
};

/* ---------------------------------------------------- */

template<class T>
class Widget : public WidgetBase,
               public UiMessagesListener {
public:
  Widget();
  int create();
  int destroy();
  int setup();
  int update();
  int draw();
  int sendUiMessage(const UiMessage& msg);
  int setUiMessageListener(UiMessagesListener* lis);                  
  void onUiMessage(const UiMessage& m);                               /* Will be called by our `UiMessages` member. `UiMessages` will make sure that the received message is safe to sure between threads. */

private:
  T* obj;
  UiMessages qt_messages;
};

/* ---------------------------------------------------- */

template<class T>
Widget<T>::Widget()
  :obj(nullptr)
{
  qt_messages.setListener(this);
}

template<class T>
int Widget<T>::create() {
  
  if (nullptr != obj) {
    qFatal("Widget<T>::create() - already created the object.");
    return -1;
  }

  obj = new T();

  return 0;
}

template <class T>
int Widget<T>::destroy() {

  if (nullptr == obj) {
    qFatal("Widget<T>::destroy() - already destroyed.");
    return -1;
  }

  obj->destroy();

  delete obj;
  obj = nullptr;

  return 0;
}

template<class T>
int Widget<T>::setup() {
  
  if (nullptr == obj) {
    qFatal("Widget<T>::setup() - not created.");
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
  internal buffer and then calls `Widget<T>::onUiMessage()` for
  each of the messages which have been added. We simply forward these
  `UiMessages` to the widget too in our `onUiMessage()` handler. So
  we're making an extra step here, but hiding synchronisation
  implementation of `UiMessages`. We can simply assume that we can do
  whatever we want with the `UiMessage`s that we receive in
  `onUiMessage`.

  `messages->notify()` will call `Widget<T>::onUiMessage()` and we
  forward the received message to `obj->onUiMessage()`.

 */

template<class T>
int Widget<T>::update() {
  
  if (nullptr == obj) {
    qFatal("Widget<T>::update() - not created.");
    return -1;
  }

  qt_messages.notify();
  
  obj->update();
  
  return 0;
}

template<class T>
int Widget<T>::draw() {
  
  if (nullptr == obj) {
    qFatal("Widget<T>::draw() - not created.");
    return -1;
  }

  obj->draw();
  
  return 0;
}

template<class T>
int Widget<T>::sendUiMessage(const UiMessage& msg) {
  qt_messages.addMessage(msg);
  return 0;
}

template<class T>
void Widget<T>::onUiMessage(const UiMessage& m) {

  if (nullptr == obj) {
    qFatal("Received a UiMessage but obj is nullptr.");
    return;
  }
  
  obj->onUiMessage(m);
}


template<class T>
int Widget<T>::setUiMessageListener(UiMessagesListener* lis) {

  if (nullptr == obj) {
    qFatal("Cannot set UiMessageListener because obj is nullptr.");
    return -1;
  }

  obj->setUiMessageListener(lis);

  return 0;
}

/* ---------------------------------------------------- */

#endif
