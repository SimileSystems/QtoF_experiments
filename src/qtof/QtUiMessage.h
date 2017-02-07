#ifndef QT_UI_MESSAGE_H
#define QT_UI_MESSAGE_H

#include <QObject>

class QtUiMessage : public QObject {
  Q_OBJECT

public:
  QtUiMessage();

  enum UiMsg {
    NONE = 0,
    WINDOW_RESIZED = 1,
    PIXEL_RATIO_CHANGED = 2,
    MOUSE_MOVE = 3,
    MOUSE_PRESS = 4,
    MOUSE_RELEASE = 5,
    MOUSE_ENTER = 6,
    MOUSE_LEAVE = 7,
    KEY_PRESS = 9,
    KEY_RELEASE = 10,
    SIZE_CHANGED = 11,
    POSITION_CHANGED = 12,
    DIRECTORY_CREATE = 1000,
    DIRECTORY_CREATED = 1010,
    DIRECTORY_DELETE = 1001,
    DIRECTORY_DELETED = 1011
  };

  Q_ENUM(UiMsg);

};

#endif
