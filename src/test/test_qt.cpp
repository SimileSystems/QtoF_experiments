/*
  This file was used during the development and experiments with Qt
  and openFrameworks
 */

#include <stdio.h>
#include <stdlib.h>
#include <QApplication.h>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
/*
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/Widgets.h>
#include <qtof/WidgetTypes.h>
#include <qtof/QtUiMessage.h>
#include <qtof/QtWidgetType.h>
#include <experimental/WidgetHistogramPimpl.h>
#include <experimental/WidgetWebcamPimpl.h>
#include <experimental/WidgetDepthKitPimpl.h>
#include <experimental/Widget3DPimpl.h>
#include <experimental/WidgetDebugPimpl.h>
*/
/* ---------------------------------------------------- */

int main(int argc, char* argv[]) {

  qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{function} - %{message}");

  QGuiApplication app(argc, argv);

  /*
  qmlRegisterType<QtOfExternal>("cc.openframeworks", 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>("cc.openframeworks", 1, 0, "QtOfExternalWidget");
  qmlRegisterType<QtUiMessage>("cc.openframeworks", 1, 0, "QtUiMessage");
  qmlRegisterType<QtWidgetType>("cc.openframeworks", 1, 0, "QtWidgetType");
  */
  /*
  widgets_add_factory_for_type(WIDGET_TYPE_HISTOGRAM, new WidgetFactory<WidgetHistogramPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_WEBCAM, new WidgetFactory<WidgetWebcamPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_DEPTHKIT, new WidgetFactory<WidgetDepthKitPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_3D, new WidgetFactory<Widget3dPimpl>());
  widgets_add_factory_for_type(WIDGET_TYPE_DEBUG, new WidgetFactory<WidgetDebugPimpl>());
  */

  QSurfaceFormat format;
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);
  
  QQmlApplicationEngine engine;
  engine.addImportPath(app.applicationDirPath() +"/plugins/");
  engine.load(QUrl("qrc:/TestForWidget.qml"));

  return app.exec();
}

/* ---------------------------------------------------- */

#if 0
  engine.load(QUrl("qrc:/TestList.qml"));
  engine.load(QUrl("qrc:/main.qml"));
#endif
