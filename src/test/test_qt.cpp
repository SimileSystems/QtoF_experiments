/*

 ----

  QtOfExternal {
  }

  QtOfExternalWidget {
   id: myclass
   width: 100
   height: 100
  }

  QtQtExternalWidget {
   id: depthkit_3d
   width: 200 
   height: 200
  }

 ---

 - We also need a factory becuase a QQuickItem may be destroyed / recreated. 

  of_external_register("myclass", myclass_setup, myclass_update, myclass_draw);
  of_external_register("depthkit_3d", dk_setup, dk_update, dk_draw);

  qtof_register<MyClass>(ref); 

 ----
 
 Generic handler for OF<>QT interop: 

   - initializes the ofExternal
   - sets up the ofAppExternal instance (with programmable renderer) 
   - handles inter interop between GL/Window and OF.
   - uses the ofAppExternal

 class ofQt {
 public:
   void onMouseMove() { 
      of_external_event(EVENT_TYPE_MOUSE_MOVE, (void*)mouse_data);
   }

   void onMousePress() { 
      of_external_event(EVENT_TYPE_MOUSE_PRESS, (void*)mouse_data);
   }

   void onMouseRelease() { 
      of_external_event(EVENT_TYPE_MOUSE_RELEASE, (void*)mouse_data);
   }

 };

 ----

 Widget that uses the `ofQt` and `ofExternal` instance
 - 

 class ofQtWidget {
 public:

   void onPaint() {
     of_external_update(myname);
     of_external_draw(myname);
   } 

 };

 ----

 */

#include <stdio.h>
#include <stdlib.h>
#include <QApplication.h>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtOfWidgets.h>
#include <experimental/WidgetHistogramPimpl.h>
#include <experimental/WidgetWebcamPimpl.h>
#include <experimental/WidgetDepthKitPimpl.h>
#include <experimental/Widget3DPimpl.h>
#include <experimental/WidgetDebugPimpl.h>

/* ---------------------------------------------------- */

int main(int argc, char* argv[]) {

  qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{function} - %{message}");

  QGuiApplication app(argc, argv);

  qmlRegisterType<QtOfExternal>("cc.openframeworks", 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>("cc.openframeworks", 1, 0, "QtOfExternalWidget");

  qtof_widget_add(1, new QtOfWidget<WidgetHistogramPimpl>());
  qtof_widget_add(2, new QtOfWidget<WidgetWebcamPimpl>());
  qtof_widget_add(3, new QtOfWidget<WidgetDepthKitPimpl>());
  qtof_widget_add(4, new QtOfWidget<Widget3dPimpl>());
  qtof_widget_add(5, new QtOfWidget<WidgetDebugPimpl>());

  QSurfaceFormat format;
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);
  
  QQmlApplicationEngine engine;
  engine.load(QUrl("qrc:/TestList.qml"));
  //engine.load(QUrl("qrc:/main.qml"));
  return app.exec();
}


/* ---------------------------------------------------- */
