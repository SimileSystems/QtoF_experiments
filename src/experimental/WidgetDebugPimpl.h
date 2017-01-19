#ifndef WIDGET_DEBUG_PIMPL_H
#define WIDGET_DEBUG_PIMPL_H

class WidgetDebug;
class UiMessage;
class UiMessagesListener;

class WidgetDebugPimpl {
public:
  WidgetDebugPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
  void setUiMessageListener(UiMessagesListener* lis);

private:
  WidgetDebug* ctx;
};

#endif
