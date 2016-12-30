#ifndef QT_RENDER_H
#define QT_RENDER_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLFunctions>
#include <Api.h>

class QtRender : public QQuickItem, protected QOpenGLFunctions {
  Q_OBJECT
  
public:
  QtRender();
            
public slots:
  void onPaint();
  void onCleanup();
  void onSync();

private slots:
  void onWindowChanged(QQuickWindow* win);

private:
  RenderContext* ctx;
  bool is_init;
};

#endif
