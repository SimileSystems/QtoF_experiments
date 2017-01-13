import QtQuick 2.4
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import cc.openframeworks 1.0

ApplicationWindow {
  id: page
  width: 640
  height: 480
  color: "lightgray"
  title: "Test app"
  visible: true

  /*
  QtRender {
    id: of
    visible: true
    width: 640
    height: 480
  }
  */

  Rectangle {
    id: panel
    width: 300
    opacity: 0.5
    anchors.top: parent.top
    /* anchors.left: parent.left */
    anchors.bottom: parent.bottom
    anchors.margins: 20
    color: "#FFFFFF"
    visible: true

    PropertyAnimation {
      id: animation
      target: panel
      properties: "x"
      from: -panel.width
      to: panel.anchors.margins
      duration: 360
      easing.type: Easing.OutCubic
    }
    /*
    MouseArea {
      anchors.fill: parent;
      onClicked: {
        animation.running = true
        panel.visible =  true
        column.visible = true
      }
    }
    */
  }
  
  Column {

    id: column
    anchors.top: panel.top
    anchors.left: panel.left
    anchors.right: panel.right
    anchors.margins: 10
    spacing: 10
    visible: true
    
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

    /*
    Button {
      text: "Delete"
      anchors.right: parent.right
      anchors.left: parent.left
      onClicked: {
        histogram.destroy();
      }
    }

    Button {
      text: "Create"
      anchors.right: parent.right
      anchors.left: parent.left
      onClicked: {
        Qt.createQmlObject("import cc.openframeworks 1.0; QtOfExternalWidget { ref: 1; width: 120; height: 120; x: 310; y: 10;}", page, "test");
      }
    }

    Button {
      text: "Unset parent"
      anchors.right: parent.right
      anchors.left: parent.left
      onClicked: {
        histogram.parent = null;
      }
    }
    */
    
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

    ComboBox {
      anchors.right: parent.right
      anchors.left: parent.left
      model: [ ".mp4", ".mov", ".h264", ".h265" ]
    }

    Row {
      spacing: 10
      Label {
        text: "Red"
      }
      Slider {
        id: red_color
        value: 0.5
        onValueChanged: {
          histogram.sendExternalEventFloat(100, red_color.value);
        }
      }
    }
    Row {
      spacing: 10
      Label {
        text: "Green"
      }
      Slider {
        id: green_color
        value: 0.5
        onValueChanged: {
          histogram.sendExternalEventFloat(101, green_color.value);
        }
      }
    }
  }

  QtOfExternal {
    visible: true
    gl_major_version: 3
    gl_minor_version: 3
  }


  QtOfExternalWidget {
    id: webcam
    ref: 2
    x: 0
    y: 0
    width: 640
    height: 480
  }
  
  QtOfExternalWidget {
    id: histogram
    ref: 1
    width: 320
    height: 240
    x: 310
    y: 20
    focus: true // Set focus so we can accept key events. 
  }

  /*
    FastBlur {
    anchors.fill: histogram
    source: histogram
    radius: 32
    }
  */
  
  /*
    Drag.active: dragArea.drag.active
    Drag.hotSpot.x: 0
    Drag.hotSpot.y: 0
    
    MouseArea {
    id: dragArea
    anchors.fill: parent
    drag.target: parent
    }
  */

  /*
  Item {
    width: 200; height: 200


    DropArea {
      x: 75; y: 75
      width: 50; height: 50
      visible: true

      Rectangle {
        anchors.fill: parent
        color: "green"

        visible: parent.containsDrag
      }
    }

    Rectangle {
      x: 10; y: 10
      width: 20; height: 20
      color: "red"

      Drag.active: dragArea.drag.active
      Drag.hotSpot.x: 10
      Drag.hotSpot.y: 10

      MouseArea {
        id: dragArea
        anchors.fill: parent

        drag.target: parent
      }
    }
  }
  */
}
