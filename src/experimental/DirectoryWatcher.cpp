#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <ofMain.h>
#include <experimental/DirectoryWatcher.h>

/* ----------------------------------------------------- */

static void watch_thread(DirectoryWatcher* w);
static void on_uv_event(uv_fs_event_t *handle, const char *filename, int events, int status);
  
/* ----------------------------------------------------- */

DirectoryWatcher::DirectoryWatcher()
  :listener(nullptr)
{
}

int DirectoryWatcher::init(const std::string& dir, DirectoryWatcherListener* lis) {
  
  if (0 == dir.size()) {
    printf("DirectoryWatcher::init() - error: path is empty.\n");
    return -1;
  }

  if (nullptr == lis) {
    printf("DirectoryWatcher::init() - error: given listener is NULL.\n");
    return -2;
  }

  path = dir;
  listener = lis;

  if (0 != fillDirectoryList(directories)) {
    printf("DirectoryWatcher::init() - error: failed to fill directory list.\n");
    return -2;
  }

  for (auto existing_dir : directories) {
    listener->onDirectoryCreated(existing_dir);
  }
  
  th = std::thread(watch_thread, this);
  th.detach();
  
  return 0;
}

int DirectoryWatcher::updateAndNotifyDirectoryList() {

  if (0 == path.size()) {
    printf("DirectoryWatcher::updateAndNotifyDirectoryList() - not initialized.\n");
    return -1;
  }

  if (nullptr == listener) {
    printf("DirectoryWatcher::updateAndNotifyDirectoryList() - listener is nullptr.\n");
    return -2;
  }

  /* Check what directories we have. */
  std::set<std::string> new_list;
  if (0 != fillDirectoryList(new_list)) {
    printf("DirectoryWatcher::updateAndNotifyDirectoryList() - failed ot fill dir list.\n");
    return -2;
  }

  /* Check what directories have been removed. */
  std::set<std::string> diff_removed;
  std::set_difference(directories.begin(), directories.end(), new_list.begin(), new_list.end(), std::inserter(diff_removed, diff_removed.begin()));
  for (auto removed_dir : diff_removed) {
    listener->onDirectoryRemoved(removed_dir);
  }
    
  /* Check what directories have been created. */
  std::set<std::string> diff_new;
  std::set_difference(new_list.begin(), new_list.end(), directories.begin(), directories.end(), std::inserter(diff_new, diff_new.begin()));
  for (auto new_dir : diff_new) {
    listener->onDirectoryCreated(new_dir);
  }

  /* And update our list. */
  directories = new_list;
  
  return 0;
}

/* ----------------------------------------------------- */

int DirectoryWatcher::fillDirectoryList(std::set<std::string>& result) {
  
  if (0 == path.size()) {
    printf("DirectoryWatcher::fillDirectoryList() - not initialized.\n");
    return -1;
  }

  ofDirectory of_dir(path);
  of_dir.listDir();

  for (size_t i = 0; i < of_dir.size(); ++i) {
    ofDirectory d(of_dir.getPath(i));
    if (false == d.isDirectory()) {
      continue;
    }
    result.insert(of_dir.getPath(i));
  }

  return 0;
}

/* ----------------------------------------------------- */
/*

  Note: when an error occures we exit so we don't take care of
  freeing allocated memory.

 */
static void watch_thread(DirectoryWatcher* w) {

  int r = 0;
  uv_loop_t* loop = nullptr;
  uv_fs_event_t* event_req = nullptr;
  
  if (nullptr == w) {
    printf("DirectoryWatcher - watch_thread: `w` is nullptr. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 == w->path.size()) {
    printf("DirectoryWatcher - watch_thread: path is empty. (exiting).\n");
    exit(EXIT_FAILURE);
  }
  
  loop = uv_loop_new();
  if (nullptr == loop) {
    printf("DirectoryWatcher - watch_thread: failed to create a loop handle. (exiting). \n");
    exit(EXIT_FAILURE);
  }

  r = uv_loop_init(loop);
  if (0 != r) {
    printf("DirectoryWatcher - watch_thread: failed to init our loop handle. (exiting).\n");
    printf("-> %s", uv_strerror(r));
    exit(EXIT_FAILURE);
  }

  event_req = (uv_fs_event_t*)malloc(sizeof(uv_fs_event_t));
  if (nullptr == event_req) {
    printf("DirectoryWatcher - watch_thread: failed to allocate event obj. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  event_req->data = (void*)w;
  
  r = uv_fs_event_init(loop, event_req);
  if (0 != r) {
    printf("DirectoryWatcher - watch_thread: failed to init the fs event. (exiting).\n");
    printf("-> %s", uv_strerror(r));
    exit(EXIT_FAILURE);
  }

  r = uv_fs_event_start(event_req, on_uv_event, w->path.c_str(), 0);
  if (0 != r) {
    printf("DirectoryWatcher - watch_thread: failed to start watching for dir changes. (exiting).\n");
    printf("-> %s\n", uv_strerror(r));
    exit(EXIT_FAILURE);
  }

  uv_run(loop, UV_RUN_DEFAULT);
  
  return ;
}

/* ----------------------------------------------------- */

static void on_uv_event(uv_fs_event_t *handle, const char *filename, int events, int status) {
  
  DirectoryWatcher* w = static_cast<DirectoryWatcher*>(handle->data);
  if (nullptr == w) {
    printf("DirectoryWatcher - on_uv_event: failed to cast user data into DirectoryWatcher. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  w->updateAndNotifyDirectoryList();
}

/* ----------------------------------------------------- */
