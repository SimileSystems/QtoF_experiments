#ifndef UI_MESSAGES_H
#define UI_MESSAGES_H

#include <stdint.h>
#include <string>
#include <vector>
#include <mutex>

/* ------------------------------------------------------ */

#define UI_MSG_NONE 0
#define UI_MSG_WINDOW_RESIZED 1
#define UI_MSG_PIXEL_RATIO_CHANGED 2
#define UI_MSG_MOUSE_MOVE 3
#define UI_MSG_MOUSE_PRESS 4
#define UI_MSG_MOUSE_RELEASE 5
#define UI_MSG_MOUSE_ENTER 6
#define UI_MSG_MOUSE_LEAVE 7
#define UI_MSG_KEY_PRESS 9
#define UI_MSG_KEY_RELEASE 10
#define UI_MSG_SIZE_CHANGED 11
#define UI_MSG_POSITION_CHANGED 12

/* ------------------------------------------------------ */

class UiMessage {
public:
  UiMessage();
  
public:
  uint32_t type;
  int32_t i[4];
  float f[4];
  std::string s;
};

/* ------------------------------------------------------ */

class UiMessagesListener {
public:
  virtual void onUiMessage(const UiMessage& m) = 0;
};

/* ------------------------------------------------------ */

class UiMessages {
public:
  UiMessages();
  int setListener(UiMessagesListener* lis);
  void notify();
  void addMessage(const UiMessage& m);

private:
  UiMessagesListener* listener;
  std::mutex mtx;
  std::vector<UiMessage> messages;
};

/* ------------------------------------------------------ */
#endif
