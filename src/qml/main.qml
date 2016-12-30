import QtQuick 2.4
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import tv.depthkit 1.0

ApplicationWindow {
  id: page
  width: 640
  height: 480
  color: "lightgray"
  title: "Test app"
  visible: true

  Rectangle {
    id: panel
    width: 300
    opacity: 0.5
    anchors.top: parent.top
    /* anchors.left: parent.left */
    anchors.bottom: parent.bottom
    anchors.margins: 20
    color: "darkblue"
    visible: false

    PropertyAnimation {
      id: animation
      target: panel
      properties: "x"
      from: -panel.width
      to: panel.anchors.margins
      duration: 360
      easing.type: Easing.OutCubic
    }
  }

  MouseArea {
    anchors.fill: parent;
    onClicked: {
      animation.running = true
      panel.visible =  true
      column.visible = true
    }
  }

  Column {

    id: column
    anchors.top: panel.top
    anchors.left: panel.left
    anchors.right: panel.right
    anchors.margins: 10
    spacing: 10
    visible: false
    
    Text {
      id: text
      text: "Settings"
      font.pointSize: 16
      font.bold: true
    }

    TextInput {
      id: input2
      anchors.right: parent.right
      anchors.left: parent.left
      text: "Text input..."
    }

    Button {
      text: "Save"
      anchors.right: parent.right
      anchors.left: parent.left
    }
    
    Button {
      text: "Load"
      anchors.right: parent.right
      anchors.left: parent.left
    }

    CheckBox {
      text: "Enable triangulation"
    }

    CheckBox {
      text: "Enable surface reconstruction"
    }
    
    CheckBox {
      text: "Export depth images"
    }

    ComboBox {
      anchors.right: parent.right
      anchors.left: parent.left
      model: [ ".mp4", ".mov", ".h264", ".h265" ]
    }
  }

  QtRender {
    visible: true
  }



}
