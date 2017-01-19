import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import cc.openframeworks 1.0

ApplicationWindow {
  id: app
  width: 1280
  height: 720
  title: "Depth Kit - List Test"
//  color: "lightgray" /* @todo Have to set the background color; otherwise the widgets are not drawn? */
  visible: true

  QtOfExternal {
    visible: true
    gl_major_version: 3
    gl_minor_version: 3
  }


  ListModel {
    id: scenes_model
    ListElement {
      number: "1"
      title: "Take name 001"
      duration: "06:34"
    }
    ListElement {
      number: "2"
      title: "Take name 002"
      duration: "02:11"
    }
  }

  QtOfExternalWidget {
    id: depthkit
    ref: 3
    layer: 0
    width: app.width
    height: app.height
    x: 0
    y: 0
    function onUiMessage(msg) {
      if (msg.type == 1000) {
        scenes_model.insert(0, {title: "TEST", number: "X", duration:"10:35"})
      }
    }
  }

  QtOfExternalWidget {
    id: histogram
    ref: 1
    layer: 1
    width: 100
    height: 100
    x: 600
    y: 100
  }

  Component {
    id: scenes_delegate
    Item {
      height: 33
      width: parent.width
      Column {
        anchors.left: parent.left
        anchors.right: parent.right
        Row {
          spacing: 10
          bottomPadding: 3
          rightPadding: 30
          Label {
            width: 30
            text: number
            color: "#33FFFFFF"
            horizontalAlignment: Text.AlignRight

            MouseArea {
              anchors.fill: parent
              hoverEnabled: true
              cursorShape: Qt.PointingHandCursor
              onClicked: {
                scenes_model.remove(index);
              }
              onEntered: {
                parent.color = "#FF0000"
              }
              onExited: {
                parent.color = "#33FFFFFF"
              }
            }
          }
          Label {
            text: title
            color: "white"
            width: 200
            anchors.leftMargin: 20
          }
          Label {
            text: duration
            color: "#33FFFFFF"
            horizontalAlignment: Text.AlignRight
          }
        }
        Row {
          anchors.left: parent.left
          anchors.right: parent.right
          Rectangle {
            color: "white"
            width: parent.width
            height: 1
            opacity: 0.5
          }
        }
      }
    }
  }
  
  Rectangle {
    id: scenes_lv
    width: 300
    color: "#CC333333"
    x: 0
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    states: [
      State {
        name: "show"
        PropertyChanges {
          target: scenes_lv
          x: 0
        }
      },
      State {
        name: "hidden"
        PropertyChanges {
          target: scenes_lv
          x: -width
        }
      }
    ]

    transitions: [
      Transition {
        NumberAnimation { property: "x"; duration: 160 }
      }
    ]

    Column {
      x: 0
      ListView {
        id: lv
        width: 300
        height: 500
        model: scenes_model
        delegate: scenes_delegate
        topMargin: 20
        add: Transition {
          NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 260 }
          NumberAnimation { properties: "y"; duration: 260; easing.type: Easing.InCirc }
        }
        displaced: Transition {
          NumberAnimation { properties: "x,y"; duration: 160; easing.type: Easing.OutCirc }
          NumberAnimation { property: "opacity"; to: 1.0}
        }
      }

      QtOfExternalWidget {
        id: d3
        ref: 4
        layer: 1
        width: parent.width
        height: 200
      }
    }
  }

  Rectangle {
    id: controls
    width: 300
    color: "#333"
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    Column {
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 20
      topPadding: 20
      spacing: 5
      Button {
        text: "Add Scene Row"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        onClicked: {
          depthkit.sendUiMessageString(1000, "Some scene");
        }
      }
      Button {
        text: "Hide scenes"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        onClicked: {
          scenes_lv.state = "hidden"
          console.log("HIDE!")
        }
      }
      Button {
        text: "Show scenes"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        onClicked: {
          scenes_lv.state = "show"
        }
      }
      Button {
        text: "Tigger UiMessage"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        onClicked: {
          depthkit.sendUiMessageString(203, "somestring");
        }
      }
    }
  }
}
