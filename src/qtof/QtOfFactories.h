/*

  QtOfFactories
  =============

  GENERAL INFO:

    The QtOfFactories are tightly used together with our custom QML
    item: `QtOfExternalWidget`. The `QtOfFactories` is used to create,
    setup, update, draw etc. your custom OF based widget (it can
    actually be any object that you want to use, but we focus on OF
    here). Qt Quick may at any point destroy or recreate a QML
    item. Therefore we use this helper to do just that. 

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
    between this `ref` value and your type using `qtof_factory_add();`.
    For example to map the ref value `1` to your histogram widget
    you would call:

    `qtof_factory_add(1, new QtOfFactory<DepthKitHistogramWidgetPimpl>());`

    The QML would look something like: 

    ````qml
      QtOfExternalWidget {
        ref: 1
      }
   ```

   Now, when Qt creates your `QtOfExternalWidget` item, the
   `QtOfExternalWidget` implementation will call the
   `qtof_factory_*()` functions to create, update, draw, etc.. your
   custom type.

 */
#ifndef QT_OF_FACTORY_H
#define QT_OF_FACTORY_H

#include <QDebug>
#include <unordered_map>

/* ---------------------------------------------------- */

class QtOfFactoryBase {
public:
  virtual int create() = 0;
  virtual int setup() = 0;
  virtual int update() = 0;
  virtual int draw() = 0;
};

/* ---------------------------------------------------- */

template<class T>
class QtOfFactory : public QtOfFactoryBase {
public:
  QtOfFactory();
  int create();
  int setup();
  int update();
  int draw();

private:
  T* obj;
};

/* ---------------------------------------------------- */

class QtOfFactories {
public:
  QtOfFactories();
  int add(int ref, QtOfFactoryBase* fac);
  int create(int ref);
  int setup(int ref);
  int update(int ref);
  int draw(int ref);
  
private:
  std::unordered_map<int, QtOfFactoryBase*> factories;
};

/* ---------------------------------------------------- */

extern QtOfFactories qtof_factories;

int qtof_factory_add(int ref, QtOfFactoryBase* fac);
int qtof_factory_create(int ref);
int qtof_factory_setup(int ref);
int qtof_factory_update(int ref);
int qtof_factory_draw(int ref);

/* ---------------------------------------------------- */

template<class T>
QtOfFactory<T>::QtOfFactory()
  :obj(nullptr)
{
}

template<class T>
int QtOfFactory<T>::create() {
  
  if (nullptr != obj) {
    qFatal("QtOfFactory::create() - already created the object.");
    return -1;
  }

  obj = new T();

  return 0;
}

template<class T>
int QtOfFactory<T>::setup() {
  
  if (nullptr == obj) {
    qFatal("QtOfFactory::setup() - not created.");
    return -1;
  }

  obj->setup();
  
  return 0;
}

template<class T>
int QtOfFactory<T>::update() {
  
  if (nullptr == obj) {
    qFatal("QtOfFactory::update() - not created.");
    return -1;
  }

  obj->update();
  
  return 0;
}

template<class T>
int QtOfFactory<T>::draw() {
  
  if (nullptr == obj) {
    qFatal("QtOfFactory::draw() - not created.");
    return -1;
  }

  obj->draw();
  
  return 0;
}

/* ---------------------------------------------------- */

inline int QtOfFactories::create(int ref) {
  
  std::unordered_map<int, QtOfFactoryBase*>::iterator it = factories.find(ref);
  if (it == factories.end()) {
    qFatal("QtFactories::create() - reference not found.");
    return -1;
  }

  return it->second->create();
}

inline int QtOfFactories::setup(int ref) {
  
  std::unordered_map<int, QtOfFactoryBase*>::iterator it = factories.find(ref);
  if (it == factories.end()) {
    qFatal("QtFactories::setup() - reference not found.");
    return -1;
  }

  return it->second->setup();
}
                                          
inline int QtOfFactories::update(int ref) {

  std::unordered_map<int, QtOfFactoryBase*>::iterator it = factories.find(ref);
  if (it == factories.end()) {
    qFatal("QtFactories::update() - reference not found.");
    return -1;
  }

  return it->second->update();
}

inline int QtOfFactories::draw(int ref) {

  std::unordered_map<int, QtOfFactoryBase*>::iterator it = factories.find(ref);
  if (it == factories.end()) {
    qFatal("QtFactories::draw() - reference not found.");
    return -1;
  }

  return it->second->draw();
}

#endif
