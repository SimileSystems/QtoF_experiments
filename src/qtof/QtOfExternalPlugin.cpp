#include <qtof/QtOfExternalPlugin.h>
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtUiMessage.h>
#include <qtof/QtWidgetType.h>

/* 
   @todo this is a quickfix to make our plugin work on Windows. I've
   to figure out how/where the symbols should be stored; apparently
   there are some differences between Windows and Mac.
 */
#if defined(_WIN32)
#  include <qtof/Widgets.h>
#  include <qtof/WidgetTypes.h>
#  include <experimental/WidgetHistogramPimpl.h>
#  include <experimental/WidgetWebcamPimpl.h>
#  include <experimental/WidgetDepthKitPimpl.h>
#  include <experimental/Widget3DPimpl.h>
#  include <experimental/WidgetDebugPimpl.h>
#endif

void QtOfExternalPlugin::registerTypes(const char* uri) {

  if (nullptr == uri) {
    qFatal("Given uri is nullptr.");
    exit(EXIT_FAILURE);
  }

#if defined(_WIN32)
  widgets_add_factory_for_type(WIDGET_TYPE_HISTOGRAM, new WidgetFactory<WidgetHistogramPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_WEBCAM, new WidgetFactory<WidgetWebcamPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_DEPTHKIT, new WidgetFactory<WidgetDepthKitPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_3D, new WidgetFactory<Widget3dPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_DEBUG, new WidgetFactory<WidgetDebugPimpl>());
#endif
  
  qmlRegisterType<QtOfExternal>(uri, 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>(uri, 1, 0, "QtOfExternalWidget");
  qmlRegisterType<QtUiMessage>(uri, 1, 0, "QtUiMessage");
  qmlRegisterType<QtWidgetType>(uri, 1, 0, "QtWidgetType");
}
