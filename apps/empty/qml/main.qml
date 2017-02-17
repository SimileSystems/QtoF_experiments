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

  Rectangle {
    anchors.fill: parent
    color: "#666666"
  }
 
  Rectangle {
    width: 100
    height: 100
    color: "red"
  }
}
