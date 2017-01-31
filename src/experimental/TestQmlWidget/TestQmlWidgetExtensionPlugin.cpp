#include <experimental/TestQmlWidget/TestQmlWidgetExtensionPlugin.h>

void TestQmlWidgetExtensionPlugin::registerTypes(const char* uri) {
  //printf("Registering TestQmlWidget!!\n");
  qmlRegisterType<TestQmlWidget>(uri, 1, 0, "TestQmlWidget");
}
