#ifndef TEST_QML_WIDGET_EXTENSION_PLUGIN_H
#define TEST_QML_WIDGET_EXTENSION_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <experimental/TestQmlWidget/TestQmlWidget.h>

class TestQmlWidgetExtensionPlugin : public QQmlExtensionPlugin {
  
  Q_OBJECT;
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid);

public:
  void registerTypes(const char* uri);
};

#endif
