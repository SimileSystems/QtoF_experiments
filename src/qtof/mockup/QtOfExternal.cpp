#include <qtof/mockup/QtOfExternal.h>
#include <QPainter>


QtOfExternal::QtOfExternal(QQuickItem* parent)
  :QQuickPaintedItem(parent)
  ,gl_minor_version(0)
  ,gl_major_version(0)
  ,img(":img/of-logo.png")
{
}

void QtOfExternal::paint(QPainter* p) {
  setWidth(250);
  setHeight(75);

  p->fillRect(contentsBoundingRect(),QColor(238, 57, 135));
  p->drawImage(contentsBoundingRect(), img);
  p->drawText(10, 20, "Qt openFrameworks");
}
