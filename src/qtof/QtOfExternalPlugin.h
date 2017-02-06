#ifndef QT_OF_EXTERNAL_PLUGIN_H
#define QT_OF_EXTERNAL_PLUGIN_H

#include <QQmlExtensionPlugin>

class QtOfExternalPlugin : public QQmlExtensionPlugin {
  Q_OBJECT;
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid);
  
public:
  void registerTypes(const char* uri);
};

#endif
