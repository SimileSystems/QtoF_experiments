#pragma once

#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickPaintedItem>
#include <QScreen>
#include <qtof/ofAppExternalWindow.h>
#include <ofMain.h>

class QtOfBaseApp : public QQuickPaintedItem, public ofBaseApp {

  Q_OBJECT
  
public:
  QtOfBaseApp(QQuickItem *parent = 0);
  ~QtOfBaseApp();

  void paint(QPainter * p);

  void triggerRepaint(bool force = true);

  enum RepaintMode
  {
      OnInputEvents = 0,
      OnFocused     = 1,
      Always        = 2
  };

public slots:
  void onBeforeRender();
  void onBeforeRenderDirect();
  void onSync();
  void onCleanup();

private slots:
  void onWindowChanged(QQuickWindow* win);
  void onWidthChanged(int w);
  void onHeightChanged(int h);
  void onScreenChanged(QScreen* screen);

protected:
  void preRender();
  void render();
  void postRender();

  void mousePressEvent(QMouseEvent* ev);
  void mouseReleaseEvent(QMouseEvent* ev);
  void hoverEnterEvent(QHoverEvent* ev);
  void hoverLeaveEvent(QHoverEvent* ev);
  void hoverMoveEvent(QHoverEvent* ev);
  void mouseMoveEvent(QMouseEvent* ev);
  void wheelEvent(QWheelEvent *ev);
  void keyPressEvent(QKeyEvent* ev);
  void keyReleaseEvent(QKeyEvent* ev);
  std::shared_ptr<ofAppExternalWindow> getOfWindowRef();
  
  bool is_created;
  std::shared_ptr<ofAppExternalWindow> ofWindow; // holds reference to the ofAppExternalWindow object

  int qtToOfMouseButton(Qt::MouseButton button);
  void qtToOfKey(QKeyEvent * ev, int & key, int & keycode, int & scancode, uint32_t & codepoint);

  RepaintMode repaintMode;
  bool shouldRepaint, repaintFbo, updateFbo;

  ofFbo internalOfFbo;
  ofTexture *internalFboTexture;
  int ofFboSamples;
  GLint ofFboFormat;
private:
  void resetOpenGlState();
  void registerEvents();
  void deregisterEvents();
  bool isQtWindowEventsConnected;
  ofMutex eventsMutex;
};
