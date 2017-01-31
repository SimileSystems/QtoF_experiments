#include <experimental/TestQmlWidgetQtDesigner.h>
#include <experimental/TestQmlWidget.h>

TestQmlWidgetQtDesigner::TestQmlWidgetQtDesigner(QObject* parent)
  :QObject(parent)
  ,is_initialized(false)
{
}

bool TestQmlWidgetQtDesigner::isContainer() const {
  return false;
}

bool TestQmlWidgetQtDesigner::isInitialized() const {
  return is_initialized;
}

void TestQmlWidgetQtDesigner::initialize(QDesignerFormEditorInterface *core) {
  
  if (true == is_initialized) {
    return;
  }

  is_initialized = true;
}

QIcon TestQmlWidgetQtDesigner::icon() const {
  return QIcon();
}

QString TestQmlWidgetQtDesigner::domXml() const {
  return ""
    "<ui language=\"c++\">\n"
    " <widget class=\"TestQmlWidget\" name=\"testQmlWidget\">\n"
    "  <property name=\"geometry\">\n"
    "   <rect>\n"
    "    <x>0</x>\n"
    "    <y>0</y>\n"
    "    <width>100</width>\n"
    "    <height>100</height>\n"
    "   </rect>\n"
    "  </property>\n"
    " </widget>\n"
    "</ui>";
}

QString TestQmlWidgetQtDesigner::group() const {
  return "Display Widgets [Examples]";
}

QString TestQmlWidgetQtDesigner::includeFile() const {
  return "<experimental/TestQmlWidget.h>";
}

QString TestQmlWidgetQtDesigner::name() const {
  return "TestQmlWidget";
}

QString TestQmlWidgetQtDesigner::toolTip() const {
  return "Experimenting with exposing a custom item into Qt Designer";
}

QString TestQmlWidgetQtDesigner::whatsThis() const {
  return "TestQmlWidget - whatsThis()";
}

QWidget* TestQmlWidgetQtDesigner::createWidget(QWidget *parent) {
  return new TestQmlWidget(NULL);
}

