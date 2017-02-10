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

  Component.onCompleted:  {
    console.log("Loaded: " +QtWidgetType.HISTOGRAM)
  }

  QtOfExternal {
    x: 276
    y: 72
    gl_major_version: 3
    gl_minor_version: 3
  }


  QtOfExternalWidget {
    id: test
    x: 276
    y: 178
    width: 100
    height: 100
    level: 1
    ref: 3
    widget: QtWidgetType.HISTOGRAM
  }
  

  QtOfExternalWidget {
    id: histogram
    ref: 4
    level: 1
    widget: QtWidgetType.DEBUG
    width: 400
    height: 452
    x: 389
    y: 72
    focus: true // Set focus so we can accept key events.
  }
/*
  QtOfExternalWidget {
      id: qtOfExternalWidget
      x: 526
      y: 247
      width: 77
      height: 101
      widget: 3
      level: 1
      ref: 1
  }
*/
  
  /*  
      QtOfExternalWidget {
      id: qtOfExternalWidget
      widget: QtWidgetType.HISTOGRAM
      ref: 6
      x: 30
      y: 30
      width: 200
      height: 200
      }

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
      id: dk
      ref: 4
      type: QtWidgetType.DEPTHKIT
      width: 100
      height: 200
      x: 102
      y: 20
      }

      QtOfExternalWidget {
      id: timelement
      type: QtWidgetType.TIM
      ref: 3456
      level: 0
      width: 500 
      height: 100
      x: 300
      y: 200
      visible: true
      }
  */
}
