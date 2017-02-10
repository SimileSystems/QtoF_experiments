#ifndef ROXLU_PIMPL_H
#define ROXLU_PIMPL_H

class Roxlu;
class UiMessage;
class UiMessagesListener;

class RoxluPimpl {
public:
  RoxluPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
  void setUiMessageListener(UiMessagesListener* lis);

private:
  Roxlu* ctx;
};

#endif
