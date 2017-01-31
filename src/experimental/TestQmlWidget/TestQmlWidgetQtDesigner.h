/* 
   NOT USED

   This was created while looking into a way to expose custom
   qml items into Qt Quick Designer ... this code is used for 
   widgets and Qt Designer (not Qt Quick Designer).

 */
#ifndef TEST_QML_WIDGET_QT_DESIGNER_H
#define TEST_QML_WIDGET_QT_DESIGNER_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class TestQmlWidgetQtDesigner : public QObject,
                                public QDesignerCustomWidgetInterface
{
  Q_OBJECT;
  Q_PLUGIN_METADATA(IID "com.roxlu.TestQmlWidgetQtDesigner");
  Q_INTERFACES(QDesignerCustomWidgetInterface);
  
public:
  TestQmlWidgetQtDesigner(QObject* parent = 0);

  bool isContainer() const Q_DECL_OVERRIDE;
  bool isInitialized() const Q_DECL_OVERRIDE;
  QIcon icon() const Q_DECL_OVERRIDE;
  QString domXml() const Q_DECL_OVERRIDE;
  QString group() const Q_DECL_OVERRIDE;
  QString includeFile() const Q_DECL_OVERRIDE;
  QString name() const Q_DECL_OVERRIDE;
  QString toolTip() const Q_DECL_OVERRIDE;
  QString whatsThis() const Q_DECL_OVERRIDE;
  QWidget* createWidget(QWidget *parent) Q_DECL_OVERRIDE;
  void initialize(QDesignerFormEditorInterface *core) Q_DECL_OVERRIDE;

private:
  bool is_initialized;
};

#endif
