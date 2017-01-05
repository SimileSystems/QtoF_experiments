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

/* ---------------------------------------------------- */
