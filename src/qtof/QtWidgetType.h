#ifndef QT_WIDGET_TYPE_H
#define QT_WIDGET_TYPE_H

#include <QObject>

class QtWidgetType : public QObject {
  Q_OBJECT

public:
  QtWidgetType();

  enum WidgetType {
    NONE = 0,
    HISTOGRAM = 1,
    WEBCAM = 2,
    DEPTHKIT = 3,
    D3 = 4,
    DEBUG = 5,
    TIM = 6
  };

  Q_ENUM(WidgetType);

};

#endif
