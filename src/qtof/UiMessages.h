/*
  UI MESSAGES
  ============

  GENERAL INFO: 
  
    This file defines the types that we use to pass messages
    around. The `UiMessage` class is very basic and the usage is based
    on the implementation and up to the user. When using messages
    you've to be carefull how you use it and what members you set. For
    example, we use the member `i` for mouse events where `i[0]` is
    the x-coordinate and `i[1]` the y-coordinate. We have chosen this
    approach because we didn't want to create lots of members for the
    `UiMessage` type as that would mean more memory usage (and cpu)
    which could have influence on the performance.

  USAGE: 

    You create a `UiMessages` object, then start adding messages using
    `addMessage()`. Then if you want to hand over the added messages
    to the listener you have to call `notify()`.  Note that you can
    add multiple messages from different threads.
    
    In our Qt based applications, we call `notify()` from the
    rendering thread (e.g. when the openGL context is active), as is
    done with all interaction with the Widgets.
 
  DEFINE MESSAGE TYPES IN `UiMessageTypes.h`
  
    As a rule, you'll have to define all the different message types
    in one header called `UiMessageTypes.h`. Each widget can and
    should add new types into this file if needed. To be able to use
    the message types in QML we created some Javascript which
    generates a QML type that exposes the `#defines` in QML. See
    `qtof/CodeGenerator.js` for more info.

  UIMESSAGE IN QML:

     A Widget can send a message back to QML. The `QtOfExternalWidget`
     implements the `UiMessagesListener` interface and is added to the
     `UiMessages` member of the widgets. Whenever the
     `QtOfExternalWidget` receives a `UiMessage` it converts this into
     an object that is available in Javascript. See
     `QtOfExternalWidget.cpp` for more info.

 */
#ifndef UI_MESSAGES_H
#define UI_MESSAGES_H

#include <stdint.h>
#include <string>
#include <vector>
#include <mutex>
#include <qtof/UiMessageTypes.h>

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
