#include <QDebug>
#include <qtof/Widgets.h>

/* ---------------------------------------------------- */

Widgets widgets_manager;

/* ---------------------------------------------------- */

Widgets::Widgets() {
}

int Widgets::addFactoryForType(int type, WidgetFactoryBase* factory) {

  if (nullptr == factory) {
    qFatal("Given factory is NULL.");
    return -1;
  }

  std::unordered_map<int, WidgetFactoryBase*>::iterator it = factories.find(type);
  if (it != factories.end()) {
    qFatal("There already exists a factory for the given type");
    return -2;
  }

  factories[type] = factory;

  return 0;
}

int Widgets::createInstanceForTypeAndRef(int type, int ref) {

  std::unordered_map<int, WidgetFactoryBase*>::iterator fit = factories.find(type);
  if (fit == factories.end()) {
    qFatal("No factory found for given type: %d", type);
    return -1;
  }

  std::unordered_map<int, WidgetInstanceBase*>::iterator iit = instances.find(ref);
  if (iit != instances.end()) {
    qFatal("We already created an widget instance with the same reference ID. Use unique reference values.");
    return -2;
  }

  WidgetInstanceBase* instance = fit->second->createWidgetInstance();
  if (nullptr == instance) {
    qFatal("Failed to create an instance for the given widget type: %d, and ref: %d", type, ref);
    return -3;
  }

  instances[ref] = instance;

  return 0;
}

/* 
   This function will destroy and cleanup `WidgetFactoryBase`. We will
   not remove the `WidgetFactoryBase*` from our `widgets` map, because we
   may still need this `WidgetFactoryBase` wrapper to recreate the mapped
   widget at some other point (Qt can destroy QQuickItems at certain
   points.) This basically boils down to calling the `destroy()` and 
   destructor of your widget type. 

 */
int Widgets::destroyInstanceWithRef(int ref) {

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::destroy() failed, no instance found for reference: %d", ref);
    return -1;
  }

  it->second->destroy();

  delete it->second;
  it->second = nullptr;

  instances.erase(it);

  return 0;
}

int Widgets::setupInstanceWithRef(int ref) {

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::setup() failed, no instance found for reference: %d", ref);
    return -1;
  }

  it->second->setup();

  return 0;
}

int Widgets::sendUiMessageToInstanceWithRef(int ref, const UiMessage& msg) {

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::sendUiMessage() failed, no instance found for reference: %d", ref);
    return -1;
  }

  if (0 != it->second->sendUiMessage(msg)) {
    qFatal("Widgets::sendUiMessage() failed, widget returned an error. references: %d", ref);
    return -2;
  }
  
  return 0;
}

int Widgets::getNumRegisteredWidgets() {
  return (int) factories.size();
}

int Widgets::getNumCreatedWidgets() {
  return instances.size();
}

int Widgets::setUiMessageListenerForInstanceWithRef(int ref, UiMessagesListener* lis) {

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::setUiMessageListener() failed, no instance found for reference: %d", ref);
    return -1;
  }

  if (0 != it->second->setUiMessageListener(lis)) {
    qFatal("Widgets::setUiMessageListener() - failed.");
    return -2;
  }

  return 0;
}

/* ---------------------------------------------------- */

int widgets_add_factory_for_type(int type, WidgetFactoryBase* fac) {
  return widgets_manager.addFactoryForType(type, fac);
}

int widgets_create_instance_for_type_and_ref(int type, int ref) {
  return widgets_manager.createInstanceForTypeAndRef(type, ref);
}

int widgets_destroy_instance_with_ref(int ref) {
  return widgets_manager.destroyInstanceWithRef(ref);
}

int widgets_setup_instance_with_ref(int ref) {
  return widgets_manager.setupInstanceWithRef(ref);
}

int widgets_update_instance_with_ref(int ref) {
  return widgets_manager.updateInstanceWithRef(ref);
}

int widgets_draw_instance_with_ref(int ref) {
  return widgets_manager.drawInstanceWithRef(ref);
}

int widgets_send_message_to_instance_with_ref(int ref, const UiMessage& msg) {
  return widgets_manager.sendUiMessageToInstanceWithRef(ref, msg);
}

int widgets_get_num_registered_widgets() {
  return widgets_manager.getNumRegisteredWidgets();
}

int widgets_get_num_created_widgets() {
  return widgets_manager.getNumCreatedWidgets();
}

int widgets_set_message_listener_for_instance_with_ref(int ref, UiMessagesListener* lis) {
  return widgets_manager.setUiMessageListenerForInstanceWithRef(ref, lis);
}

/* ---------------------------------------------------- */
