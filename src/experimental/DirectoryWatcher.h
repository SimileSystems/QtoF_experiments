#ifndef DIRECTORY_WATCHER_H
#define DIRECTORY_WATCHER_H

#include <thread>
#include <string>
#include <set>
#include <uv.h>

/* ----------------------------------------------------- */

class DirectoryWatcherListener {
public:
  virtual void onDirectoryRemoved(const std::string& path) = 0; /* Called from different thread. */
  virtual void onDirectoryCreated(const std::string& path) = 0; /* Called from different thread. */
};

/* ----------------------------------------------------- */

class DirectoryWatcher {
public:
  DirectoryWatcher();
  int init(const std::string& path, DirectoryWatcherListener* lis);
  int updateAndNotifyDirectoryList();
  
private:
  int fillDirectoryList(std::set<std::string>& result);
  
public:
  std::thread th;
  std::string path;
  std::set<std::string> directories;

private:
  DirectoryWatcherListener* listener;
};

/* ----------------------------------------------------- */

#endif
