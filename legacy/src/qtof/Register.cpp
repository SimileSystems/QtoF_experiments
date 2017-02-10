#include <qtof/Register.h>
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>

void qtof_register() {
  qmlRegisterType<QtOfExternal>("qtof", 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>("qtof", 1, 0, "QtOfExternalWidget");
}
