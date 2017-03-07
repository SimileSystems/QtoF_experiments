/*

  OPENFRAMEWORKS INSIDE QT
  =========================

  GENERAL INFO:

    This custom Qt Quick 2.0 item is used to integrate OF inside a Qt
    application using Qt Quick 2.0. Qt Quick is used together with a
    markup / scripting language called QML. See [this page][0] for
    more info.

    This QML item is used to instantiate `ofExternal`. ofExternal is a
    C-style opaque API that will setup openFrameworks in such a way,
    that you can use the OF features. Basically the `ofExternal` and
    the `of_external_*()` function will setup a
    `ofAppExternalWindow()` instance and loads the GL functions (so we
    assume an active context).

    This `QtOfExternal` QML type will handle the `ofExternal` setup,
    but it will also handle generic mouse and keyboard events. Here we
    basically implement what an external window/gl handler would do
    when using the `ofAppExternalWindow`.

    For more info about how OF is used as inside Qt see `ofExternal.h`
    and `ofAppExternalWindow`.

  USAGE: 

    If you want to use OF with external widgets, your QML file must
    always contain one `QtOfExternal` item and this must be added
    before any of the `QtOfExternalWidget` items that you define in
    your QML (see `QtOfExternalWidget`). By adding this `QtOfExternal`
    item, we make sure that the openFrameworks library is correctly
    initialized. See [this][1] gist that shows a minimal QML file that
    can be used to setup OF and create an external widget.

  THREADING:

    When a `QQuickItem` instance is created (e.g. by loading a qml
    file), the instance lives in the GUI thread. This is also known as
    the main-thread. Rendering may take place in a seperate thread,
    called the rendering-thread. This means that when we call any of
    the OF functions (which potentially use openGL) we need to make
    sure that it's done from the rendering-thread. This is why we use
    the `Qt::DirectConnection` when connecting to a couple of the
    `QQuickWindow` signals. The `Qt::DirectConnection` signal type
    must be used when you want the slot to be called directly when the
    signal is emitted; instead of buffered and called from the
    GUI-thread (aka main-thread).

    The following signals are used with a `Qt::DirectConnections`:

    - QQuickWindow::beforeRendering
    - QQuickWindow::beforeSynchronizing
    - QQuickWindow::scenegraphInvalidated

  MOUSE EVENTS:
  
    Unlike the width, height, screen change signals that we receive
    we have to use the `mouse*Event()` functions to receive mause
    events. The documentation is not really clear from which thread
    these are called, but I assume these are called from the GUI
    thread.

    To be able to receive mouse events, we have to call
    `setAcceptedMouseButtons()` and `setAcceptHoverEvents()`. The
    hover events are used to detect mouse-moves.

  SCREEN CHANGED EVENT:
  
    We are handling the screen-changed events which gives us the
    opportunity to get the pixel ratio for the current screen. We need
    to be aware of a funny situation where we receive a screen-changed
    event from Qt, but no window, and no openGL context has been
    created yet. When this happens we should ignore this screen
    changed event. We use `of_external_is_init()` to check if we
    should handle the screen changed event or not.  When
    `of_external_is_init()` returns 0, it means it's initialized and
    that the openGL context has been created.

  REFERENCES:

    [0]: http://doc.qt.io/qt-5/qtquick-index.html "Qt Quick"
    [1]: https://gist.github.com/roxlu/1dea9cc36291d7a98a97f3527ba81523 "How to use `QtOfExternal` as a QML item to setup OF."
    
 */
#ifndef QT_OF_EXTERNAL_H
#define QT_OF_EXTERNAL_H

#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickItem>
#include <QScreen>

/* ---------------------------------------------------- */

class QtOfExternal : public QQuickItem {
  Q_OBJECT;
  Q_PROPERTY(int gl_minor_version WRITE setGlMinorVersion READ getGlMinorVersion)
  Q_PROPERTY(int gl_major_version WRITE setGlMajorVersion READ getGlMajorVersion)

public:
  QtOfExternal();

public slots:
  void onSync();
  void onCleanup();
  void onPaint();

protected:
  void mousePressEvent(QMouseEvent* ev);
  void mouseReleaseEvent(QMouseEvent* ev);
  void mouseMoveEvent(QMouseEvent* ev);
  void hoverMoveEvent(QHoverEvent* ev);
  void hoverEnterEvent(QHoverEvent* ev);                                      
  void hoverLeaveEvent(QHoverEvent* ev);

private slots:
  void onWindowChanged(QQuickWindow* win);
  void onWidthChanged(int w);
  void onHeightChanged(int h);
  void onScreenChanged(QScreen* screen);
  void notifyPixelRatio();                                 /* This function will send a `UI_MSG_PIXEL_RATIO_CHANGED` message when the screen on which the window sits is changed. */

public:
  void setGlMinorVersion(int v);
  void setGlMajorVersion(int v);
  int getGlMinorVersion();
  int getGlMajorVersion();

private:
  int gl_minor_version;
  int gl_major_version;
};

/* ---------------------------------------------------- */

inline void QtOfExternal::setGlMinorVersion(int v) {
  gl_minor_version = v;
}

inline int QtOfExternal::getGlMinorVersion() {
  return gl_minor_version;
}

inline void QtOfExternal::setGlMajorVersion(int v) {
  gl_major_version = v;
}

inline int QtOfExternal::getGlMajorVersion() {
  return gl_major_version;
}

/* ---------------------------------------------------- */

#endif
