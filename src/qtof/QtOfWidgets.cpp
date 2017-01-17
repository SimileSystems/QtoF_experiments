#include <QDebug>
#include <qtof/QtOfWidgets.h>

/* ---------------------------------------------------- */

QtOfWidgets qtof_widgets;

/* ---------------------------------------------------- */

QtOfWidgets::QtOfWidgets() {

}

int QtOfWidgets::add(int ref, QtOfWidgetBase* fac) {

  if (nullptr == fac) {
    qFatal("Given factory is NULL.");
    return -1;
  }

  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it != widgets.end()) {
    qFatal("There already exists a factory for the given reference");
    return -2;
  }

  widgets[ref] = fac;
  
  return 0;
}

int QtOfWidgets::create(int ref) {
  
  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::create() - reference not found.");
    return -1;
  }

  return it->second->create();
}

/* 
   This function will destroy and cleanup `QtOfWidgetBase`. We will
   not remove the `QtOfWidgetBase*` from our `widgets` map, because we
   may still need this `QtOfWidgetBase` wrapper to recreate the mapped
   widget at some other point (Qt can destroy QQuickItems at certain
   points.) This basically boils down to calling the `destroy()` and 
   destructor of your widget type. 

 */
int QtOfWidgets::destroy(int ref) {
  
  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::destroy() - reference not found.");
    return -1;
  }

  if (0 != it->second->destroy()) {
    qFatal("QtOfWidgets::destroy() - failed.");
  }

  return 0;
}

int QtOfWidgets::setup(int ref) {
  
  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::setup() - reference not found.");
    return -1;
  }
  
  return it->second->setup();
}

int QtOfWidgets::sendEvent(int ref, const ofExternalEvent& ev) {
  std::lock_guard<std::mutex> lg(mtx_events);
  events[ref].push_back(ev);
  return 0;
}

int QtOfWidgets::sendUiMessage(int ref, const UiMessage& msg) {

  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::sendUiMessage() - reference not found.");
    return -1;
  }

  it->second->sendUiMessage(msg);
  
  return 0;
}

int QtOfWidgets::getNumWidgets() {
  return (int) widgets.size();
}

int QtOfWidgets::getJson(int ref, int what, std::string& result) {
  
  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::getJson() - reference not found.");
    return -1;
  }

  if (0 != it->second->getJson(what, result)) {
    qFatal("QtOfWidgets::getJson() - failed.");
    return -2;
  }

  return 0;
}

int QtOfWidgets::setUiMessageListener(int ref, UiMessagesListener* lis) {
  
  std::unordered_map<int, QtOfWidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("QtOfWidgets::setUiMessageListener() - reference not found.");
    return -1;
  }

  if (0 != it->second->setUiMessageListener(lis)) {
    qFatal("QtOfWidgets::setUiMessageListener() - failed.");
    return -2;
  }

  return 0;
}

/* ---------------------------------------------------- */

int qtof_widget_add(int ref, QtOfWidgetBase* fac) {
  return qtof_widgets.add(ref, fac);
}

int qtof_widget_create(int ref) {
  return qtof_widgets.create(ref);
}

int qtof_widget_destroy(int ref) {
  return qtof_widgets.destroy(ref);
}

int qtof_widget_setup(int ref) {
  return qtof_widgets.setup(ref);
}

int qtof_widget_update(int ref) {
  return qtof_widgets.update(ref);
}

int qtof_widget_draw(int ref) {
  return qtof_widgets.draw(ref);
}

int qtof_widget_send_event(int ref, const ofExternalEvent& ev) {
  return qtof_widgets.sendEvent(ref, ev);
}

int qtof_widget_send_message(int ref, const UiMessage& msg) {
  return qtof_widgets.sendUiMessage(ref, msg);
}

int qtof_widget_get_num_widgets() {
  return qtof_widgets.getNumWidgets();
}

int qtof_widget_get_json(int ref, int what, std::string& json) {
  return qtof_widgets.getJson(ref, what, json);
}

int qtof_widget_set_message_listener(int ref, UiMessagesListener* lis) {
  return qtof_widgets.setUiMessageListener(ref, lis);
}
/* ---------------------------------------------------- */
