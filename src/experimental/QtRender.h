#ifndef QT_RENDER_H
#define QT_RENDER_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLFunctions>
#include <QScreen>
#include <Api.h>

class QtRender : public QQuickItem, protected QOpenGLFunctions {
  Q_OBJECT
  
public:
  QtRender();

protected:
  void mousePressEvent(QMouseEvent* ev);
  void mouseReleaseEvent(QMouseEvent* ev);
  void mouseMoveEvent(QMouseEvent* ev);
  void hoverMoveEvent(QHoverEvent* ev);
  void hoverEnterEvent(QHoverEvent* ev);                                      
  void hoverLeaveEvent(QHoverEvent* ev);
                                       
public slots:
  void onPaint();
  void onCleanup();
  void onSync();
                                       
private slots:
  void onWindowChanged(QQuickWindow* win);
  void onWidthChanged(int w);
  void onHeightChanged(int h);
  void onScreenChanged(QScreen* screen);
  
private:
  //int convertQtMouseButtonToOpenFrameworksButton(Qt::MouseButton& bt);

private:
  RenderContext* ctx;
  bool is_init;
};

#if 0
inline int QtRender::convertQtMouseButtonToOpenFrameworksButton(Qt::MouseButton& bt) {
  
  if (Qt::LeftButton == ev->button()) {
    return 
  }
  else if (Qt::MiddleButton == ev->button()) {
    button = 1;
  }
  else if (Qt::RightButton == ev->button()) {
    button = 2;
  }
  else {
    printf("Error: unhandled button.\n");
    return;
  }
}
#endif

#endif
