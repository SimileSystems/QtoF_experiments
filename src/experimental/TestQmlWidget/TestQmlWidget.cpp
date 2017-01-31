#include <QPainter>
#include <experimental/TestQmlWidget/TestQmlWidget.h>

TestQmlWidget::TestQmlWidget(QQuickItem* parent)
  :QQuickPaintedItem(parent)
{
}

void TestQmlWidget::paint(QPainter* p) {
  p->fillRect(contentsBoundingRect(), Qt::red);
}

