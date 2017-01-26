#ifndef WIDGET_DEPTH_KIT_H
#define WIDGET_DEPTH_KIT_H

#include <ofMain.h>
#include <qtof/ofExternalWidget.h>
#include <experimental/DirectoryWatcher.h>

class UiMessage;

class WidgetDepthKit : public ofExternalWidget,
                       public DirectoryWatcherListener {
public:
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
  void onDirectoryRemoved(const std::string& path);
  void onDirectoryCreated(const std::string& path);
  
private:
  DirectoryWatcher dir_watcher;
};

#endif
