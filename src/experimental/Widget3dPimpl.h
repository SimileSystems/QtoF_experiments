#ifndef WIDGET_3D_PIMPL_H
#define WIDGET_3D_PIMPL_H

class Widget3d;
class UiMessage;
class UiMessagesListener;

class Widget3dPimpl {
public:
  Widget3dPimpl();
  void setup();
  void update();
  void draw();
  void destroy();
  void onUiMessage(const UiMessage& msg);
  void setUiMessageListener(UiMessagesListener* lis);

private:
  Widget3d* ctx;
};

#endif
