#ifndef QTOF_MOCKUP_QTOFEXTERNAL_H
#define QTOF_MOCKUP_QTOFEXTERNAL_H

#include <QtQuick/QQuickPaintedItem.h>
#include <QImage>

/* ---------------------------------------------------- */

class QtOfExternal : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(int gl_minor_version WRITE setGlMinorVersion READ getGlMinorVersion)
  Q_PROPERTY(int gl_major_version WRITE setGlMajorVersion READ getGlMajorVersion)

public:
  /* Rendering */
  QtOfExternal(QQuickItem* parent = 0);
  void paint(QPainter* p);

  /* Properties */
  void setGlMinorVersion(int v);
  void setGlMajorVersion(int v);
  int getGlMinorVersion();
  int getGlMajorVersion();

private:
  int gl_minor_version;
  int gl_major_version;
  QImage img;
};

/* ---------------------------------------------------- */

inline void QtOfExternal::setGlMinorVersion(int v) {
  gl_minor_version = v;
}

inline int QtOfExternal::getGlMinorVersion() {
  return gl_minor_version;
}

inline void QtOfExternal::setGlMajorVersion(int v) {
  gl_major_version = v;
}

inline int QtOfExternal::getGlMajorVersion() {
  return gl_major_version;
}

/* ---------------------------------------------------- */


#endif
