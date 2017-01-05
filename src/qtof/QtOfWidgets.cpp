#include <QDebug>
#include <qtof/QtOfWidgets.h>

/* ---------------------------------------------------- */

QtOfWidgets qtof_factories;

/* ---------------------------------------------------- */

QtOfWidgets::QtOfWidgets() {

}

int QtOfWidgets::add(int ref, QtOfWidgetBase* fac) {

  if (nullptr == fac) {
    qFatal("Given factory is NULL.");
    return -1;
  }

  std::unordered_map<int, QtOfWidgetBase*>::iterator it = factories.find(ref);
  if (it != factories.end()) {
    qFatal("There already exists a factory for the given reference");
    return -2;
  }

  factories[ref] = fac;
  
  return 0;
}

int QtOfWidgets::sendEvent(int ref, const ofExternalEvent& ev) {
  std::lock_guard<std::mutex> lg(mtx_events);
  events[ref].push_back(ev);
  return 0;
}

/* ---------------------------------------------------- */

int qtof_widget_add(int ref, QtOfWidgetBase* fac) {
  return qtof_factories.add(ref, fac);
}

int qtof_widget_create(int ref) {
  return qtof_factories.create(ref);
}

int qtof_widget_setup(int ref) {
  return qtof_factories.setup(ref);
}

int qtof_widget_update(int ref) {
  return qtof_factories.update(ref);
}

int qtof_widget_draw(int ref) {
  return qtof_factories.draw(ref);
}

int qtof_widget_send_event(int ref, const ofExternalEvent& ev) {
  return qtof_factories.sendEvent(ref, ev);
}

/* ---------------------------------------------------- */
