#ifndef QTOF_MOCKUP_QTOFPLUGIN_H
#define QTOF_MOCKUP_QTOFPLUGIN_H

#include <QQmlExtensionPlugin>

class QtOfPlugin : public QQmlExtensionPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid);

public:
  void registerTypes(const char* uri);
};

#endif
