/*

  EXTERNAL WIDGET THAT USES OPENFRAMEWORKS
  ========================================

  GENERAL INFO:

    This class defines a QML type that can be used to create, destroy,
    update, draw etc, an openFrameworks based QML item. A widget is here
    defined as an item that you can load into a QML (Qt Quick 2.0) based
    application. For example, for DepthKit we want to render a histogram
    using OF features. To make this work we create a class that follows
    some rules that we defined for a widget type.

    Because how QML works, we need to be aware that QML items can be
    created and destroyed on the fly. Therefore an OF based QML widget, 
    needs to be carefull with the resources it creates. It needs to 
    cleanup correctly. 

    Also, to be able to use OF inside QT we follow the `pointer to
    implementation` [PIMPL][0] idiom, which allows us to completely
    separate the implementation. Qt allows us to create new QML
    items. For this we have to implement a couple of functions and
    take care of how we create/destroy things. To make it easier and
    to separate the Qt features from the OF features, I created this
    `QtOfExternalWidget` that works together with `Widgets`.
    The `QtOfExternalWidget` is like a bridge between this custom QML
    item and your OF based widget.

    The `Widgets` creates, sets up, updates, draws, etc... our custom
    OF based widgets. Using this setup, we don't have to create and
    define a new QML item for each widget we want to create. Though
    because this `QtOfExternalWidget` still needs to know what
    OF-based widget it needs to update, render, etc.. we are using a
    numeric identifier, that we call `ref`. The first draft of this
    class required the user to set the `ref` property through QML.  We
    removed this requirement and we simply generate a reference in the
    constructor.


    +----------------------+
    |                      |
    |  QtOfExternalWidget  o---| Generic QML item for OF based widgets.
    |                      |
    +----------+-----------+
               |
               |
    +----------v-----------+
    |                      |
    |     Widgets          o---| Helper to create/update/draw/destroy OF 
    |                      |     based PIMPL widgets. 
    +----------+-----------+
               |
               |
    +----------v-----------+
    |                      |
    |    ofWidgetPimpl     o---| The actual widget that uses OF code. 
    |                      |     We need PIMPL because of GL header 
    +----------+-----------+     issues (included from Qt, OF, etc..)
               |
               |
    +----------v-----------+
    |                      |
    |      ofWidget        o---| This is your OF based widget, which is
    |                      |     exposed though your `ofWidgetPimpl`. 
    +----------------------+

  SETTING UP YOUR WIDGET:

    For certain widget you would like to set some properties before
    the `setup()` function of the widget is called. For example, lets
    say we have a webcam widget and you want to tell it what camera-id
    to use. For these kind of setup/configure options you create a
    `onSetup()` function in your QML that will be called before we
    call the `setup()` function of your widget. In `onSetup()` you can
    use the messaging functions to pass data into your C++ widget.
    Use e.g. `sendUiMessage()` or `sendUiMessageString()` in your QML
    to communicate with the C++ widget.

  EVENTS:
  
    Ths `QtOfExternalWidget` will call the `widgets_send_message()`
    function for the widget that it handles. These messages are used
    to notify a widget about mouse events, key events, size events
    ets.  Below is a list of events that are dispatched directly to
    the widget implementation.

    UI_MSG_SIZE_CHANGED:
      Is dispatched when the width and height of the item
      changes. This event is also dispatched before we call `setup` on
      the widget.

    UI_MSG_POSITION_CHANGED:
      Is dispatched when the position (x,y) of the item changes. We
      set the `xy` member of the event object. This event is also
      dispatched before we call `setup()` on the widget so it can
      define it's position.

    UI_MSG_MOUSE_LEAVE:
    UI_MSG_MOUSE_ENTER:
      Is dispatched when the user moves his mouse over or out of, the
      QML item. The `x`, `y`, `width` and `height` properties are used
      by Qt Quick to determine when the user enters an item. We set
      the `mouse[0]` and `mouse[1]` members of the event object.

    UI_MSG_PIX_RATIO_CHANGED:
      Is dispatched when the pixel ratio of the current screen
      changes. E.g. this may happen when the user moves the window
      onto another screen when the screen onto which the window is
      being dragged has different DPI then the previous screen.

  PAINTING:

    For each registered widget a new `QtOfExternalWidget` will be
    created and each `onPaint` function will be executed. In our
    `onPaint` function we have to `start` and `finish` the
    (programmable) renderer from openFrameworks.  This will clear the
    framebuffer. This is not problem when we only have one widget, but
    when we have multiple, we cannot just `finish` (and so clear) the
    renderer when the first instance of a `QtOfExternalWidget` has
    drawn it's contents.

    For each frame, We want to start the renderer only for the first
    `QtOfExternalWidget` instance and finish the renderer only for the
    last instance. To make this work, I've currently added a `static
    int paint_count` to `::onPaint()`. When `paint_count` is 0 we
    start the renderer and when `paint_count` reaches the total number
    of widgets we finish the renderer.

    Be aware that Qt does something like this for every frame:

      ````
      foreach (QtOfExternalWidget as widget) {
         widget->onPaint()
      }
      ````

  REFERENCES:

    [0]: https://en.wikipedia.org/wiki/Opaque_pointer "Opaque pointer; aka PIMPL."
    [1]: http://doc.qt.io/qt-5/qquickitem.html "Graphics Resource Handling"

 */

#ifndef QT_OF_EXTERNAL_WIDGET_H
#define QT_OF_EXTERNAL_WIDGET_H

#include <stdint.h>
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickItem>
#include <QString>
#include <QScreen>
#include <qtof/UiMessages.h>
#include <qtof/WidgetTypes.h>

/* ---------------------------------------------------- */

class QtOfExternalWidget : public QQuickItem,
                           public UiMessagesListener {
  Q_OBJECT
  Q_PROPERTY(int level WRITE setLevel READ getLevel)                  /* The level onto which we draw, 0: background, 1: foreground */
  Q_PROPERTY(int widget WRITE setWidget READ getWidget)
  
public:
  QtOfExternalWidget();
  ~QtOfExternalWidget();
  void onUiMessage(const UiMessage& msg);                             /* Is called by the `UiMessages` member of the actual widget implementation. */

public slots:
  void onPaint();
  void onSync();
  void onCleanup();

public:
  int getLevel();
  int getWidget();
  void setLevel(const int& v);
  void setWidget(const int& t);
  
  Q_INVOKABLE void sendUiMessageString(unsigned int msgType, const QString& str);
  Q_INVOKABLE void sendUiMessage(unsigned int msgType);

private slots:
  void onWindowChanged(QQuickWindow* win);
  void onScreenChanged(QScreen* screen);

private:
  void notifySize();                                                  /* Notifies the widget about it's size; is (also) called before we call `setup()` on the widget. */
  void notifyPosition();                                              /* Notifies the widget about it's position; is (also) called before we call `setup()` on the widget. */
  void notifyPixelRatio();                                            /* Notifies the widget about the current pixel ratio (e.g. to handle retina). is (also) called before we call `setup()` on the widget. */
  void resetOpenGlState();                                            /* After rendering our own things using openFrameworks, we have to reset the GL state because Qt will run into rendering issues. Qt provides a function for this which sets some of the default state; but not all. We use or own wrapper to reset some other state too. */
  void callOnSetupQmlHandler();                                       /* This function will call `onSetup()` on the QML object; this is where the user should initialize the object; we call `onSetup` before the `setup()` of the widget itself. */

protected:
  void mousePressEvent(QMouseEvent* ev);
  void mouseReleaseEvent(QMouseEvent* ev);
  void hoverEnterEvent(QHoverEvent* ev);                                      
  void hoverLeaveEvent(QHoverEvent* ev);
  void hoverMoveEvent(QHoverEvent* ev);
  void mouseMoveEvent(QMouseEvent* ev);
  void keyPressEvent(QKeyEvent* ev);
  void keyReleaseEvent(QKeyEvent* ev);
  void releaseResources();
  
private:
  int ref;
  int level;
  int widget;                                                        /* The widget type, see WidgetTypes.h */
  bool is_created; 
};

/* ---------------------------------------------------- */

inline int QtOfExternalWidget::getLevel() {
  return level;
}

inline int QtOfExternalWidget::getWidget() {
  return widget;
}

inline void QtOfExternalWidget::setLevel(const int& v) {
  level = v;
}

inline void QtOfExternalWidget::setWidget(const int& t) {
  widget = t;
}

/* ---------------------------------------------------- */

#endif
