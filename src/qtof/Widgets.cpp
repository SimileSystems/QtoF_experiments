#include <QDebug>
#include <qtof/Widgets.h>

/* ---------------------------------------------------- */

Widgets widgets_mananager;

/* ---------------------------------------------------- */

Widgets::Widgets()
  :num_created_widgets(0)
{

}

int Widgets::add(int ref, WidgetBase* fac) {

  if (nullptr == fac) {
    qFatal("Given factory is NULL.");
    return -1;
  }

  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it != widgets.end()) {
    qFatal("There already exists a factory for the given reference");
    return -2;
  }

  widgets[ref] = fac;
  
  return 0;
}

int Widgets::create(int ref) {
  
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::create() - reference not found.");
    return -1;
  }

  if (0 != it->second->create()) {
    qFatal("Widgets::create() - failed to create through pimpl.\n");
    return -2;
  }

  num_created_widgets++;

  return 0;
}

/* 
   This function will destroy and cleanup `WidgetBase`. We will
   not remove the `WidgetBase*` from our `widgets` map, because we
   may still need this `WidgetBase` wrapper to recreate the mapped
   widget at some other point (Qt can destroy QQuickItems at certain
   points.) This basically boils down to calling the `destroy()` and 
   destructor of your widget type. 

 */
int Widgets::destroy(int ref) {
  
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::destroy() - reference not found.");
    return -1;
  }

  if (0 != it->second->destroy()) {
    qFatal("Widgets::destroy() - failed.");
  }

  if (num_created_widgets <= 0) {
    qFatal("QtOfWidgers::destroy() - num_created_widgets is 0 which means we shouldn't destroy any.");
  }
  
  num_created_widgets--;

  return 0;
}

int Widgets::setup(int ref) {
  
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::setup() - reference not found.");
    return -1;
  }
  
  return it->second->setup();
}

int Widgets::sendUiMessage(int ref, const UiMessage& msg) {

  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::sendUiMessage() - reference not found.");
    return -1;
  }

  it->second->sendUiMessage(msg);
  
  return 0;
}

int Widgets::getNumRegisteredWidgets() {
  return (int) widgets.size();
}

int Widgets::getNumCreatedWidgets() {
  return num_created_widgets;
}

int Widgets::setUiMessageListener(int ref, UiMessagesListener* lis) {
  
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::setUiMessageListener() - reference not found.");
    return -1;
  }

  if (0 != it->second->setUiMessageListener(lis)) {
    qFatal("Widgets::setUiMessageListener() - failed.");
    return -2;
  }

  return 0;
}

/* ---------------------------------------------------- */

int widgets_add(int ref, WidgetBase* fac) {
  return widgets_mananager.add(ref, fac);
}

int widgets_create(int ref) {
  return widgets_mananager.create(ref);
}

int widgets_destroy(int ref) {
  return widgets_mananager.destroy(ref);
}

int widgets_setup(int ref) {
  return widgets_mananager.setup(ref);
}

int widgets_update(int ref) {
  return widgets_mananager.update(ref);
}

int widgets_draw(int ref) {
  return widgets_mananager.draw(ref);
}

int widgets_send_message(int ref, const UiMessage& msg) {
  return widgets_mananager.sendUiMessage(ref, msg);
}

int widgets_get_num_registered_widgets() {
  return widgets_mananager.getNumRegisteredWidgets();
}

int widgets_get_num_created_widgets() {
  return widgets_mananager.getNumCreatedWidgets();
}

int widgets_set_message_listener(int ref, UiMessagesListener* lis) {
  return widgets_mananager.setUiMessageListener(ref, lis);
}

/* ---------------------------------------------------- */
