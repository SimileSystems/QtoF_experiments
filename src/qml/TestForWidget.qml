import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import qtof 1.0

ApplicationWindow {

  id: app
  width: 1280
  height: 720
  title: "Qt Quick Designer Test"
  color: "#c6c6c6"
  visible: true

  QtOfExternal {
    visible: true
    gl_major_version: 3
    gl_minor_version: 3
  }

  Rectangle {
    anchors.fill: parent
    color: "#666666"
    anchors.rightMargin: 22
    anchors.bottomMargin: 112
    anchors.leftMargin: 648
    anchors.topMargin: 20
    visible: false
  }           

  QtOfExternalWidget {
    id: histogram
    ref: 1
    type: QtWidgetType.HISTOGRAM
    width: 391
    height: 197
    x: 229
    y: 20
    focus: true // Set focus so we can accept key events.
  }

  QtOfExternalWidget {
    id: debug_c
    ref: 2
    type: QtWidgetType.DEBUG
    width: 20
    height: 20
    x: 20
    y: 20
  }

  QtOfExternalWidget {
    id: debug_b
    ref: 3
    type: QtWidgetType.DEBUG
    width: 152
    height: 186
    x: 60
    y: 20
  }

  QtOfExternalWidget {
      id: qtOfExternalWidget
      type: QtWidgetType.HISTOGRAM
      x: 264
      y: 346
      width: 154
      height: 114
      ref: 6
  }

}
