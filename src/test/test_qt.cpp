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
#include <qtof/QtOfExternal.h>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/Widgets.h>
#include <qtof/WidgetTypes.h>
#include <qtof/QtUiMessage.h>
#include <experimental/WidgetHistogramPimpl.h>
#include <experimental/WidgetWebcamPimpl.h>
#include <experimental/WidgetDepthKitPimpl.h>
#include <experimental/Widget3DPimpl.h>
#include <experimental/WidgetDebugPimpl.h>
//#include <experimental/TestQmlWidget.h>

/* ---------------------------------------------------- */

int main(int argc, char* argv[]) {

  qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{function} - %{message}");

  QGuiApplication app(argc, argv);

  //qmlRegisterType<TestQmlWidget>("roxlu", 1, 0, "TestQmlWidget");
  qmlRegisterType<QtOfExternal>("cc.openframeworks", 1, 0, "QtOfExternal");
  qmlRegisterType<QtOfExternalWidget>("cc.openframeworks", 1, 0, "QtOfExternalWidget");
  qRegisterMetaType<QtUiMessage::UiMsg>("QtUiMessage::UiMsg");
  qmlRegisterType<QtUiMessage>("cc.openframeworks", 1, 0, "QtUiMessage");

  widgets_add(WIDGET_TYPE_HISTOGRAM, new Widget<WidgetHistogramPimpl>());
  widgets_add(WIDGET_TYPE_WEBCAM, new Widget<WidgetWebcamPimpl>());
  widgets_add(WIDGET_TYPE_DEPTHKIT, new Widget<WidgetDepthKitPimpl>());
  widgets_add(WIDGET_TYPE_3D, new Widget<Widget3dPimpl>());
  widgets_add(WIDGET_TYPE_DEBUG, new Widget<WidgetDebugPimpl>());

  QSurfaceFormat format;
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);
  
  QQmlApplicationEngine engine;
  engine.addImportPath(app.applicationDirPath() +"/plugins/");
  //nengine.load(QUrl("qrc:/TestList.qml"));
  engine.load(QUrl("qrc:/TestForWidget.qml"));
  //engine.load(QUrl("qrc:/main.qml"));
  return app.exec();
}


/* ---------------------------------------------------- */
