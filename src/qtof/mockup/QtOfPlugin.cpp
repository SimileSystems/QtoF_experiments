#include <qtof/mockup/QtOfPlugin.h>
#include <qtof/mockup/QtOfExternal.h>
#include <qtof/mockup/QtOfExternalWidget.h>
#include <qtof/QtUiMessage.h>
#include <qtof/QtWidgetType.h>

void QtOfPlugin::registerTypes(const char* uri) {
  qmlRegisterType<QtOfExternal>(uri, 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>(uri, 1, 0, "QtOfExternalWidget");
  qmlRegisterType<QtUiMessage>(uri, 1, 0, "QtUiMessage");
  qmlRegisterType<QtWidgetType>(uri, 1, 0, "QtWidgetType");
}

