#ifndef QTOF_MOCKUP_QTOFEXTERNALWIDGET_H
#define QTOF_MOCKUP_QTOFEXTERNALWIDGET_H

#include <QtQuick/QQuickPaintedItem.h>
#include <QImage>

/* ---------------------------------------------------- */

class QtOfExternalWidget : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(int ref MEMBER ref)
  Q_PROPERTY(int level MEMBER level)
  Q_PROPERTY(int widget MEMBER widget READ getWidget WRITE setWidget)
  
public:
  /* Rendering */
  QtOfExternalWidget(QQuickItem* parent = 0);
  void paint(QPainter* p);

  /* Invokables */
  Q_INVOKABLE void sendUiMessageString(unsigned int msgType, const QString& str);
  Q_INVOKABLE void sendUiMessage(unsigned int msgType);

  void setWidget(int w);
  int getWidget();
  
private:
  int ref;
  int level;               /* The level onto which we draw, 0: background, 1: foreground */
  int widget;              /* The widget type, see WidgetTypes.h */
};

/* ---------------------------------------------------- */

inline void QtOfExternalWidget::sendUiMessageString(unsigned int msgType, const QString& str) {
}

inline void QtOfExternalWidget::sendUiMessage(unsigned int msgType) {
}

/* ---------------------------------------------------- */

inline void QtOfExternalWidget::setWidget(int w) {
  widget = w;
  update();
}

inline int QtOfExternalWidget::getWidget() {
  return widget;
}

/* ---------------------------------------------------- */


#endif
