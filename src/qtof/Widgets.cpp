#include <QDebug>
#include <qtof/Widgets.h>

/* ---------------------------------------------------- */

Widgets widgets_manager;

/* ---------------------------------------------------- */

Widgets::Widgets()
  :num_created_widgets(0)
{

}

int Widgets::addFactoryForType(int type, WidgetBase* factory) {

  if (nullptr == factory) {
    qFatal("Given factory is NULL.");
    return -1;
  }

  std::unordered_map<int, WidgetBase*>::iterator it = factories.find(type);
  if (it != factories.end()) {
    qFatal("There already exists a factory for the given type");
    return -2;
  }

  factories[type] = factory;

  return 0;
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

int Widgets::createInstanceForTypeAndRef(int type, int ref) {

  std::unordered_map<int, WidgetBase*>::iterator fit = factories.find(type);
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
  num_created_widgets++;
  //WidgetBase* wb = new WidgetBase(it->second);

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

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::destroy() failed, no instance found for reference: %d", ref);
    return -1;
  }

  it->second->destroy();

  delete it->second;
  it->second = nullptr;

  instances.erase(it);

  
  /*
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
  */
  
  num_created_widgets--;

  return 0;
}

int Widgets::setup(int ref) {

  /*
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::setup() - reference not found.");
    return -1;
  }
  
  return it->second->setup();
  */

  std::unordered_map<int, WidgetInstanceBase*>::iterator it = instances.find(ref);
  if (it == instances.end()) {
    qFatal("Widgets::setup() failed, no instance found for reference: %d", ref);
    return -1;
  }

  it->second->setup();

  return 0;
}

int Widgets::sendUiMessage(int ref, const UiMessage& msg) {

  /*
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::sendUiMessage() - reference not found.");
    return -1;
  }

  it->second->sendUiMessage(msg);
  */

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
  return (int) widgets.size();
}

int Widgets::getNumCreatedWidgets() {
  return num_created_widgets;
}

int Widgets::setUiMessageListener(int ref, UiMessagesListener* lis) {

  /*
  std::unordered_map<int, WidgetBase*>::iterator it = widgets.find(ref);
  if (it == widgets.end()) {
    qFatal("Widgets::setUiMessageListener() - reference not found.");
    return -1;
  }

  if (0 != it->second->setUiMessageListener(lis)) {
    qFatal("Widgets::setUiMessageListener() - failed.");
    return -2;
  }
  */

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

int widgets_add_factory_for_type(int type, WidgetBase* fac) {
  return widgets_manager.addFactoryForType(type, fac);
}

int widgets_create_instance_for_type_and_ref(int type, int ref) {
  return widgets_manager.createInstanceForTypeAndRef(type, ref);
}

int widgets_add(int ref, WidgetBase* fac) {
  return widgets_manager.add(ref, fac);
}

int widgets_create(int ref) {
  return widgets_manager.create(ref);
}

int widgets_destroy(int ref) {
  return widgets_manager.destroy(ref);
}

int widgets_setup(int ref) {
  return widgets_manager.setup(ref);
}

int widgets_update(int ref) {
  return widgets_manager.update(ref);
}

int widgets_draw(int ref) {
  return widgets_manager.draw(ref);
}

int widgets_send_message(int ref, const UiMessage& msg) {
  return widgets_manager.sendUiMessage(ref, msg);
}

int widgets_get_num_registered_widgets() {
  return widgets_manager.getNumRegisteredWidgets();
}

int widgets_get_num_created_widgets() {
  return widgets_manager.getNumCreatedWidgets();
}

int widgets_set_message_listener(int ref, UiMessagesListener* lis) {
  return widgets_manager.setUiMessageListener(ref, lis);
}

/* ---------------------------------------------------- */
