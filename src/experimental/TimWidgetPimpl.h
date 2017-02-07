#ifndef TIM_WIDGET_PIMPL_H
#define TIM_WIDGET_PIMPL_H

class TimWidget;
class UiMessage;
class UiMessagesListener;

class TimWidgetPimpl {
public:
  TimWidgetPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
  void setUiMessageListener(UiMessagesListener* lis);

private:
  TimWidget* ctx;
};

#endif
