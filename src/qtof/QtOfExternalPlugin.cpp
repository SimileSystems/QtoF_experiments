#include <qtof/QtOfExternalPlugin.h>
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtUiMessage.h>
#include <qtof/QtWidgetType.h>

void QtOfExternalPlugin::registerTypes(const char* uri) {

  if (nullptr == uri) {
    qFatal("Given uri is nullptr.");
    exit(EXIT_FAILURE);
  }

  qmlRegisterType<QtOfExternal>(uri, 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>(uri, 1, 0, "QtOfExternalWidget");
  qmlRegisterType<QtUiMessage>(uri, 1, 0, "QtUiMessage");
  qmlRegisterType<QtWidgetType>(uri, 1, 0, "QtWidgetType");
}
