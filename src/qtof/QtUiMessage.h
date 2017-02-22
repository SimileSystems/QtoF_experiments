/* Generated by /Users/roxlu/Documents/programming/tests/107_qtof_usage/extern/QtoF_experiments/src/qtof/CodeGenerator.js, Tue Feb 21 2017 17:27:06 GMT+0100 (CET) */

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
    DIRECTORY_DELETED = 1011,
    EXAMPLE_RED = 1001,
    EXAMPLE_BLUE = 1002
  };

  Q_ENUM(UiMsg);

};

#endif
