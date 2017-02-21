#include <qtof/mockup/QtOfExternalWidget.h>
#include <qtof/WidgetTypeUtils.h>
#include <QPainter>

QtOfExternalWidget::QtOfExternalWidget(QQuickItem* parent)
  :QQuickPaintedItem(parent)
  ,ref(-1)
  ,level(0)
  ,widget(0)
{
  setWidth(100);
  setHeight(100);
}

void QtOfExternalWidget::paint(QPainter* p) {

  p->fillRect(contentsBoundingRect(), Qt::white);
  
  std::string name = widget_type_to_string(widget);
  p->drawText(10, 20, name.c_str());
}

