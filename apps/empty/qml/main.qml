import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import cc.openframeworks 1.0

ApplicationWindow {
  id: app
  visible: true
  width: 640
  height: 480
  title: qsTr("Hello World")
  color: "lightgray"

  QtOfExternal {
    visible: true
    gl_major_version: 3
    gl_minor_version: 3
  }

  Rectangle {
    width: 100
    height: 100
    color: "red"
  }

  
  QtOfExternalWidget {
    id: depthkit
    ref: 3
    layer: 0
    width: app.width
    height: app.height
    x: 0
    y: 0
  }    

}
