#include <qtof/QtOfExternalPlugin.h>
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtUiMessage.h>
#include <qtof/QtWidgetType.h>
#include <qtof/Widgets.h>
#include <qtof/WidgetTypes.h>

/* Include your widget PIMPL wrappers here. */
#include <experimental/WidgetHistogramPimpl.h>
#include <experimental/WidgetWebcamPimpl.h>
#include <experimental/WidgetDepthKitPimpl.h>
#include <experimental/Widget3dPimpl.h>
#include <experimental/WidgetDebugPimpl.h>
#include <experimental/TimWidgetPimpl.h>

void QtOfExternalPlugin::registerTypes(const char* uri) {

  if (nullptr == uri) {
    qFatal("Given uri is nullptr.");
    exit(EXIT_FAILURE);
  }

  /* 
     Register the PIMPL wrappers for our widgets.  When you want to
     add new widgets you need to make sure you add a call to
     `widget_add_factory_for_type()`
  */
  widgets_add_factory_for_type(WIDGET_TYPE_HISTOGRAM, new WidgetFactory<WidgetHistogramPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_WEBCAM, new WidgetFactory<WidgetWebcamPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_DEPTHKIT, new WidgetFactory<WidgetDepthKitPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_D3, new WidgetFactory<Widget3dPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_DEBUG, new WidgetFactory<WidgetDebugPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_TIM, new WidgetFactory<TimWidgetPimpl>());
  
  /* Register our custom types. */
  qmlRegisterType<QtOfExternal>(uri, 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>(uri, 1, 0, "QtOfExternalWidget");
  qmlRegisterType<QtUiMessage>(uri, 1, 0, "QtUiMessage");
  qmlRegisterType<QtWidgetType>(uri, 1, 0, "QtWidgetType");
}