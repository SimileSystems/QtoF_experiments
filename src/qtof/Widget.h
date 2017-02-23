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

class WidgetInstanceBase {
public:
  virtual ~WidgetInstanceBase() {}
  virtual void setup() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  virtual void destroy() = 0;
  virtual int sendUiMessage(const UiMessage& msg) = 0;              /* Send a UI message from the GUI to the widget. */
  virtual int setUiMessageListener(UiMessagesListener* lis) = 0;    /* Set the message listener that receives messages from the widget. (so other direction then `sendUiMessage()`. */
};

/* ---------------------------------------------------- */

class WidgetFactoryBase {
public:
  virtual WidgetInstanceBase* createWidgetInstance() = 0;
};

/* ---------------------------------------------------- */

template <class T>
class WidgetInstance : public WidgetInstanceBase,
                       public UiMessagesListener {
public:
  WidgetInstance();
  void setup();
  void update();
  void draw();
  void destroy();
  int sendUiMessage(const UiMessage& msg);
  int setUiMessageListener(UiMessagesListener* lis);
  void onUiMessage(const UiMessage& m);                               /* Will be called by our `UiMessages` member. `UiMessages` will make sure that the received message is safe to sure between threads. */
  
private:
  T* obj;
  UiMessages qt_messages;
};

/* ---------------------------------------------------- */

template<class T>
class WidgetFactory : public WidgetFactoryBase {
public:
  WidgetInstanceBase* createWidgetInstance();
};

/* ---------------------------------------------------- */

template <class T>
WidgetInstanceBase* WidgetFactory<T>::createWidgetInstance() {
  return new WidgetInstance<T>();
}

/* ---------------------------------------------------- */

template <class T>
WidgetInstance<T>::WidgetInstance()
  :obj(nullptr)
{
  obj = new T();
  qt_messages.setListener(this);
}

template <class T>
inline void WidgetInstance<T>::setup() {

  if (nullptr == obj) {
    qFatal("WidgetInstance<T>::setup() - not created.");
    return;
  }

  obj->setup();
}

/*

  This function will call the `update()` function of the widget that
  the user implemented. This can be a widget that draws a histogram
  for example. We also call `qt_messages.notify()` which will lock the
  internal buffer and then calls `WidgetInstance<T>::onUiMessage()` for each
  of the messages which have been added. We simply forward these
  `UiMessages` to the widget too in our `onUiMessage()` handler. So
  we're making an extra step here, but hiding synchronisation
  implementation of `UiMessages`. We can simply assume that we can do
  whatever we want with the `UiMessage`s that we receive in
  `onUiMessage`.

  `qt_messages->notify()` will call `WidgetInstance<T>::onUiMessage()`
  and we forward the received message to `obj->onUiMessage()`.

 */
template <class T>
inline void WidgetInstance<T>::update() {

  if (nullptr == obj) {
    qFatal("WidgetInstance<T>::update() - not created.");
    return;
  }

  qt_messages.notify();
  obj->update();
}

template <class T>
inline void WidgetInstance<T>::draw() {

  if (nullptr == obj) {
    qFatal("WidgetInstance<T>::draw() - not created.");
    return;
  }

  obj->draw();
}

template <class T>
inline void WidgetInstance<T>::destroy() {
  
  if (nullptr == obj) {
    qFatal("WidgetInstance<T>::destroy() - not created.");
    return;
  }

  obj->destroy();

  delete obj;
  obj = nullptr;
}

template <class T>
inline int WidgetInstance<T>::sendUiMessage(const UiMessage& msg) {
  qt_messages.addMessage(msg);
  return 0;
}

template <class T>
inline int WidgetInstance<T>::setUiMessageListener(UiMessagesListener* lis) {

  if (nullptr == obj) {
    qFatal("Cannot set UiMessageListener because obj is nullptr.");
    return -1;
  }

  printf("Set ui message listner\n");
  obj->setUiMessageListener(lis);

  return 0;
}

template <class T>
inline void WidgetInstance<T>::onUiMessage(const UiMessage& m) {

  if (nullptr == obj) {
    qFatal("Received a UiMessage but obj is nullptr.");
    return;
  }

  obj->onUiMessage(m);
}

/* ---------------------------------------------------- */

#endif
