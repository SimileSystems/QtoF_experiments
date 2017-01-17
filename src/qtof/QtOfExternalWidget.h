/*

  QtOfExternalWidget
  ==================

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
    `QtOfExternalWidget` that works together with `QtOfWidgets`.
    The `QtOfExternalWidget` is like a bridge between this custom QML
    item and your OF based widget.

    The `QtOfWidgets` creates, sets up, updates, draws, etc... our
    custom OF based widgets. Using this setup, we don't have to create
    and define a new QML item for each widget we want to
    create. Though because this `QtOfExternalWidget` still needs to
    know what OF-based widget it needs to update, render, etc.. we are
    using a numeric identifier, that we call `ref`. For example we can
    define the ref number `1` to a custom widget with type
    `DepthKitHistogramWidget`. Once we've registered this reference
    number with the `DepthKithHistogramWidget` type, this class
    (`QtOfExternalWidget`) will make sure that the
    `DepthKitHistogramWidget` will be created, updated, drawn,
    etc.. at the right moments. See `QtOfWidgets.h` for more info.


    +----------------------+
    |                      |
    |  QtOfExternalWidget  o---| Generic QML item for OF based widgets.
    |                      |
    +----------+-----------+
               |
               |
    +----------v-----------+
    |                      |
    |     QtOfWidgets      o---| Helper to create/update/draw/destroy OF 
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

  EVENTS:
  
    Ths `QtOfExternalWidget` will call the `onExternalEvent()` function
    of the widget. Below is a list of events that are dispatched directly
    to the widget implementation.

    OF_EXT_EVENT_SIZE_CHANGED:
      Is dispatched when the width and height of the item changes. This 
      event is also dispatched before we call `setup` on the widget.

    OF_EXT_EVENT_POSITION_CHANGED:
      Is dispatched when the position (x,y) of the item changes. We
      set the `xy` member of the event object. This event is also
      dispatched before we call `setup()` on the widget so it can 
      define it's position.

    OF_EXT_EVENT_MOUSE_LEAVE:
    OF_EXT_EVENT_MOUSE_ENTER:
      Is dispatched when the user moves his mouse over or out of, the
      QML item. The `x`, `y`, `width` and `height` properties are used
      by Qt Quick to determine when the user enters an item. We set
      the `mouse[0]` and `mouse[1]` members of the event object.

    OF_EXT_EVENT_PIX_RATIO_CHANGED:
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

/* ---------------------------------------------------- */

class QtOfExternalWidget : public QQuickItem,
                           public UiMessagesListener {
  Q_OBJECT
  Q_PROPERTY(int ref WRITE setRef READ getRef)

public:
  QtOfExternalWidget();
  ~QtOfExternalWidget();
  void onUiMessage(const UiMessage& msg);  /* Is called by the `UiMessages` member of the actual widget implementation. */

public slots:
  void onPaint();
  void onSync();
  void onCleanup();

public:
  int getRef();
  void setRef(const int& v);
#if 0  
  Q_INVOKABLE void sendExternalEventFloat(unsigned int eventType, const float& v);
  Q_INVOKABLE void sendExternalEventInt(unsigned int eventType, const int& v);
  Q_INVOKABLE QString getJson(unsigned int what);   /* Calls `getJson()` from the widget; used to exchange data between the GUI and the widget itself. */
#edif  
  Q_INVOKABLE void sendUiMessageString(unsigned int eventType, const QString& str);                                                

private slots:
  void onWindowChanged(QQuickWindow* win);
  void onScreenChanged(QScreen* screen);

private:
  void notifySize();                                /* Notifies the widget about it's size; is (also) called before we call `setup()` on the widget. */
  void notifyPosition();                            /* Notifies the widget about it's position; is (also) called before we call `setup()` on the widget. */
  void notifyPixelRatio();                          /* Notifies the widget about the current pixel ratio (e.g. to handle retina). is (also) called before we call `setup()` on the widget. */
  void resetOpenGlState();                          /* After rendering our own things using openFrameworks, we have to reset the GL state because Qt will run into rendering issues. Qt provides a function for this which sets some of the default state; but not all. We use or own wrapper to reset some other state too. */

protected:
  void hoverEnterEvent(QHoverEvent* ev);                                      
  void hoverLeaveEvent(QHoverEvent* ev);
  void keyPressEvent(QKeyEvent* ev);
  void keyReleaseEvent(QKeyEvent* ev);
  void releaseResources();
  
private:
  int ref;
  bool is_created; 
};

/* ---------------------------------------------------- */

inline int QtOfExternalWidget::getRef() {
  return ref;
}

inline void QtOfExternalWidget::setRef(const int& v) {
  ref = v;
}

/* ---------------------------------------------------- */

#endif
