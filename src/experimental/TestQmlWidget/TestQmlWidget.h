/*
  TEST QML WIDGET
  ===============

  GENERAL INFO:

    I created this class because I wanted to have a very basic and 
    simple QML item that I can use while testing different features
    of Qt, Qt Creator and Qt Designer.  This type was for example used
    while trying to figure out how we can add a custom visible item
    into the list of object into Qt Designer.

  NOTES:

    This is based on [this article][0]. 

  REFERENCES:

    [0]: http://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html "Writing QML Extensions with C++"
  
 */
#ifndef TEST_QML_WIDGET_H
#define TEST_QML_WIDGET_H

#include <QtQuick/QQuickPaintedItem>
#include <QtQuick/QQuickItem>

/* ---------------------------------------------------- */

class TestQmlWidget : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(int ref WRITE setRef READ getRef)
  Q_PROPERTY(WidgetType wtype WRITE setType READ getType DESIGNABLE true)

public:
  
  enum WidgetType {
    WIDGET_TYPE_NONE,
    WIDGET_TYPE_HISTOGRAM,
    WIDGET_TYPE_WEBCAM,
  };

  Q_ENUM(WidgetType);
  
public:
  TestQmlWidget(QQuickItem* parent = 0);
  void paint(QPainter* p);
  void setRef(const int& v);
  int getRef();

  void setType(WidgetType t) { wtype = t; }
  WidgetType getType() { return wtype; } 
  
private:
  int ref;
  WidgetType wtype;
};

/* ---------------------------------------------------- */

inline void TestQmlWidget::setRef(const int& v) {
  ref = v;
}

inline int TestQmlWidget::getRef() {
  return ref;
}

/* ---------------------------------------------------- */


#endif
