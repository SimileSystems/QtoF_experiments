#include <qtof/UiMessages.h>

/* ------------------------------------------------------ */

UiMessage::UiMessage()
  :type(UI_MSG_NONE)
{
  i[0] = 0;
  i[1] = 0;
  i[2] = 0;
  i[3] = 0;
  f[0] = 0.0f;
  f[1] = 0.0f;
  f[2] = 0.0f;
  f[3] = 0.0f;
}

/* ------------------------------------------------------ */

UiMessages::UiMessages()
  :listener(nullptr)
{
}

int UiMessages::setListener(UiMessagesListener* lis) {

  if (nullptr == lis) {
    printf("Error: UiMessages::setListener(): given listener is nullptr.\n");
    return -1;
  }

  listener = lis;
  
  return 0;
}

void UiMessages::notify() {
  
  if (nullptr == listener) {
    printf("Error: UiMessages::notify(): listener member is nullptr.");
    return;
  }
  
  std::lock_guard<std::mutex> lg(mtx);
  for (size_t i = 0; i < messages.size(); ++i) {
    if (messages[i].type == 1001) {
      printf("Notifying message type 1001\n");
    }
    listener->onUiMessage(messages[i]);
  }

  messages.clear();
}

void UiMessages::addMessage(const UiMessage& m) {
  std::lock_guard<std::mutex> lg(mtx);
  messages.push_back(m);
}

/* ------------------------------------------------------ */
