#include <stdio.h>
#include <stdlib.h>
#include <QApplication.h>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QtRender.h>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  qmlRegisterType<QtRender>("tv.depthkit", 1, 0, "QtRender");

  QSurfaceFormat format;
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(format);
  
  QQmlApplicationEngine engine;
  engine.load(QUrl("qrc:/main.qml"));
  return app.exec();
}
