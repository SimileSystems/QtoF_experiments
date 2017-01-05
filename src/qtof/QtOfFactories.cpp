#include <QDebug>
#include <qtof/QtOfFactories.h>

/* ---------------------------------------------------- */

QtOfFactories qtof_factories;

/* ---------------------------------------------------- */

QtOfFactories::QtOfFactories() {

}

int QtOfFactories::add(int ref, QtOfFactoryBase* fac) {

  if (nullptr == fac) {
    qFatal("Given factory is NULL.");
    return -1;
  }

  std::unordered_map<int, QtOfFactoryBase*>::iterator it = factories.find(ref);
  if (it != factories.end()) {
    qFatal("There already exists a factory for the given reference");
    return -2;
  }

  factories[ref] = fac;
  
  return 0;
}

int QtOfFactories::sendEvent(int ref, const ofExternalEvent& ev) {
  std::lock_guard<std::mutex> lg(mtx_events);
  events[ref].push_back(ev);
  return 0;
}

/* ---------------------------------------------------- */

int qtof_factory_add(int ref, QtOfFactoryBase* fac) {
  return qtof_factories.add(ref, fac);
}

int qtof_factory_create(int ref) {
  return qtof_factories.create(ref);
}

int qtof_factory_setup(int ref) {
  return qtof_factories.setup(ref);
}

int qtof_factory_update(int ref) {
  return qtof_factories.update(ref);
}

int qtof_factory_draw(int ref) {
  return qtof_factories.draw(ref);
}

int qtof_factory_send_event(int ref, const ofExternalEvent& ev) {
  return qtof_factories.sendEvent(ref, ev);
}

/* ---------------------------------------------------- */
