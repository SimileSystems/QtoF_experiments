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
      x: 70
      y: 36
      width: 145
      height: 76
      gl_minor_version: 3
      gl_major_version: 3
  }

  QtOfExternalWidget {
      id: qtOfExternalWidget
      x: 76
      y: 151
      width: 306
      height: 243
      widget: QtWidgetType.HISTOGRAM
      ref: 1
  }

  QtOfExternalWidget {
      id: qtOfExternalWidget1
      x: 400
      y: 23
      width: 440
      height: 294
      ref: 2
      widget: QtWidgetType.DEBUG
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

  /* lets try to use QtOfExternalWidget :) */

}
