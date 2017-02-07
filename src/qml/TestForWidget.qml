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
    gl_major_version: 3
    gl_minor_version: 3
  }

  Rectangle {
    anchors.fill: parent
    color: "#666666"
    anchors.rightMargin: 92
    anchors.bottomMargin: 118
    anchors.leftMargin: 578
    anchors.topMargin: 14
    visible: false
  }     
        
/*
  QtOfExternalWidget {
    id: histogram
    ref: 1
/*    type: QtWidgetType.HISTOGRAM */
    width: 391
    height: 197
    x: 229
    y: 20
    focus: true // Set focus so we can accept key events.
  }

  /*
  QtOfExternalWidget {
    id: debug_c
    ref: 2
    type: QtWidgetType.DEBUG
    width: 46
    height: 110
    x: 20
    y: 20
  }

  QtOfExternalWidget {
    id: debug_b
    ref: 3
    type: QtWidgetType.DEBUG
    width: 152
    height: 186
    x: 50
    y: 226
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

  QtOfExternalWidget {
    id: dk
    ref: 4
    type: QtWidgetType.DEPTHKIT
    width: 100
    height: 200
    x: 102
    y: 20
  }
  */
  
  QtOfExternalWidget {
    id: timelement
    type: QtWidgetType.TIM
    ref: 3456
    layer: 0
    width: 500 
    height: 100
    x: 300
    y: 200
	visible: true
}

}
