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
    |    ofWidgetImpl      o---| This is your OF based widget, which is
    |                      |     exposed though your `ofWidgetPimpl`. 
    +----------------------+
    

  REFERENCES:

    [0]: https://en.wikipedia.org/wiki/Opaque_pointer "Opaque pointer; aka PIMPL."

 */

#ifndef QT_OF_EXTERNAL_WIDGET_H
#define QT_OF_EXTERNAL_WIDGET_H

#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickItem>
#include <QString>
#include <QScreen>

/* ---------------------------------------------------- */

class QtOfExternalWidget : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(int ref WRITE setRef READ getRef)

public:
  QtOfExternalWidget();

public slots:
  void onPaint();
  void onSync();
  void onCleanup();

public:
  int getRef();
  void setRef(const int& v);

private slots:
  void onWindowChanged(QQuickWindow* win);
  void onScreenChanged(QScreen* screen);

private:
  void notifySize();                                /* Notifies the widget about it's size; is (also) called before we call `setup()` on the widget. */
  void notifyPosition();                            /* Notifies the widget about it's position; is (also) called before we call `setup()` on the widget. */
  void notifyPixelRatio();                          /* Notifies the widget about the current pixel ratio (e.g. to handle retina). is (also) called before we call `setup()` on the widget. */

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
