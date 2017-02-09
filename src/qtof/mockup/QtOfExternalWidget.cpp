#include <qtof/mockup/QtOfExternalWidget.h>
#include <QPainter>

QtOfExternalWidget::QtOfExternalWidget(QQuickItem* parent)
  :QQuickPaintedItem(parent)
  ,img(":img/depthkit.png")
  ,ref(-1)
  ,level(0)
  ,widget(0)
{
  setWidth(100);
  setHeight(100);
}

void QtOfExternalWidget::paint(QPainter* p) {
  p->fillRect(contentsBoundingRect(), Qt::red);
  p->drawImage(contentsBoundingRect(), img);
}

