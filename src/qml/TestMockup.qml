import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import qtof 1.0

ApplicationWindow {
  id: app
  width: 1280
  height: 720
  title: "Mockup Test"
  color: "#CCCCCC"
  visible: true

  QtOfExternal {
      id: qtOfExternal
      x: 29
      y: 45
      width: 145
      height: 76
      gl_minor_version: 3
      gl_major_version: 3
  }

  QtOfExternalWidget {
      id: qtOfExternalWidget
      x: 83
      y: 209
      width: 306
      height: 243
      widget: QtWidgetType.DEPTHKIT
      ref: 1
  }

  QtOfExternalWidget {
      id: qtOfExternalWidget1
      x: 400
      y: 23
      width: 440
      height: 294
      widget: QtWidgetType.DEBUG
      ref: 2
  }

  QtOfExternalWidget {
      id: qtOfExternalWidget2
      x: 400
      y: 334
      width: 440
      height: 352
      widget: QtWidgetType.D3
      ref: 3
  }

  QtOfExternalWidget {
      id: roxlu
      x: 927
      y: 512
      width: 176
      height: 169
      widget: QtWidgetType.DEBUG
      ref: 8
      Component.onCompleted:{
          console.log("...");
          this.sendUiMessageString(QtUiMessage.DIRECTORY_DELETE, "TESTING");
      }
  }

  /* lets try to use QtOfExternalWidget :) */

}
