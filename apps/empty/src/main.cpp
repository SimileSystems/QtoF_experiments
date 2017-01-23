#include <stdio.h>
#include <stdlib.h>
#include <QApplication.h>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtUiMessage.h>
#include <qtof/Widgets.h>
#include <experimental/WidgetDepthKitPimpl.h>

/* ---------------------------------------------------- */

int main(int argc, char* argv[]) {

  QGuiApplication app(argc, argv);

  /* Register our custom types. */
  qmlRegisterType<QtOfExternal>("cc.openframeworks", 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>("cc.openframeworks", 1, 0, "QtOfExternalWidget");
  qmlRegisterType<QtUiMessage>("cc.openframeworks", 1, 0, "QtUiMessage");

  /* Add our custom widgets */
  widgets_add(3, new Widget<WidgetDepthKitPimpl>());
  
  /* Define the GL format. */
  QSurfaceFormat format;
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);

  /* Run using our QML. */
  QQmlApplicationEngine engine;
  engine.load(QUrl("qrc:/main.qml"));
  return app.exec();
}


/* ---------------------------------------------------- */
