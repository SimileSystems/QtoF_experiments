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

 */
#ifndef QT_OF_WIDGET_H
#define QT_OF_WIDGET_H

#include <mutex>
#include <unordered_map>
#include <QDebug>
#include <qtof/ofExternal.h>

/* ---------------------------------------------------- */

class QtOfWidgetBase {
public:
  virtual int create() = 0;
  virtual int destroy() = 0;
  virtual int setup() = 0;
  virtual int update() = 0;
  virtual int draw() = 0;
  virtual int sendEvent(const ofExternalEvent& ev) = 0;
  virtual int getJson(int what, std::string& result) = 0;
};

/* ---------------------------------------------------- */

template<class T>
class QtOfWidget : public QtOfWidgetBase {
public:
  QtOfWidget();
  int create();
  int destroy();
  int setup();
  int update();
  int draw();
  int sendEvent(const ofExternalEvent& ev);
  int getJson(int what, std::string& json);

private:
  T* obj;
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
  int sendEvent(int ref, const ofExternalEvent& ev);
  int getJson(int ref, int what, std::string& result);
  int getNumWidgets(); /* Get the total number of added widgets. Is used to start and stop the underlaying (ofExternal) renderer. */
  
private:
  std::mutex mtx_events;
  std::unordered_map<int, QtOfWidgetBase*> widgets;
  std::unordered_map<int, std::vector<ofExternalEvent> > events;
};

/* ---------------------------------------------------- */

extern QtOfWidgets qtof_widgets;

int qtof_widget_add(int ref, QtOfWidgetBase* fac);
int qtof_widget_create(int ref);
int qtof_widget_destroy(int ref);
int qtof_widget_setup(int ref);
int qtof_widget_update(int ref);
int qtof_widget_draw(int ref);
int qtof_widget_send_event(int ref, const ofExternalEvent& ev);
int qtof_widget_get_num_widgets(); /* Returns the total number of registered widgets. This is used to start/stop the renderer at the right time, see `QtOfExternalWidget.cpp`  */
int qtof_widget_get_json(int ref, int what, std::string& result);

/* ---------------------------------------------------- */

template<class T>
QtOfWidget<T>::QtOfWidget()
  :obj(nullptr)
{
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

template<class T>
int QtOfWidget<T>::update() {
  
  if (nullptr == obj) {
    qFatal("QtOfWidget<T>::update() - not created.");
    return -1;
  }

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
int QtOfWidget<T>::sendEvent(const ofExternalEvent& ev) {
  
  if (nullptr == obj) {
    qFatal("Cannot send the event because obj is nullptr.");
    return -1;
  }

  obj->onExternalEvent(ev);

  return 0;
}

template<class T>
int QtOfWidget<T>::getJson(int what, std::string& result) {

  if (nullptr == obj) {
    qFatal("Cannot get json because obj is nullptr.");
    return -1;
  }

  obj->getJson(what, result);

  return 0;
}

/* ---------------------------------------------------- */

inline int QtOfWidgets::update(int ref) {

  /* Find the factory for the given ref. */
  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::update() - reference not found.");
    return -1;
  }
  QtOfWidgetBase* fac = it->second;

  /* Notify all collected events. */
  std::lock_guard<std::mutex> lg(mtx_events);
  std::unordered_map<int, std::vector<ofExternalEvent> >::iterator ev_it = events.find(ref); 
  while (ev_it != events.end()) {
    std::vector<ofExternalEvent>& obj_events = ev_it->second;
    for (size_t i = 0; i < obj_events.size(); ++i) {
      fac->sendEvent(obj_events[i]);
    }
    obj_events.clear();
    ++ev_it;
  }

  /* And finally update(). */
  return fac->update();
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
