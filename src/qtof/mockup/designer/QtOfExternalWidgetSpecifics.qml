import QtQuick 2.1
import HelperWidgets 2.0
import QtQuick.Layouts 1.0
import "QtOfWidgetNames.js" as WidgetNames

Column {
  anchors.left: parent.left
  anchors.right: parent.right

  Section {
    anchors.left: parent.left
    anchors.right: parent.right
    caption: qsTr("QtOfExternalWidget")

    SectionLayout {
      Label {
        text: qsTr("Widget")
      }

      SecondColumnLayout {
        ComboBox {
          /* model: ["HISTOGRAM", "WEBCAM", "DEPTHKIT", "D3", "DEBUG", "TIM"] */
          model: WidgetNames.get_widget_names()
          /* model: ["Histogram", "Webcam", "DepthKit", "D3", "Debug", "Tim"]*/
          backendValue: backendValues.widget
          scope: "QtWidgetType"
        }

        ExpandingSpacer {
        }
      }


      Label {
        text: "Reference ID"
      }

      SecondColumnLayout {

        SpinBox {
          backendValue: backendValues.ref
          minimumValue: 0
          maximumValue: 999999
          decimals: 0
        }

        ExpandingSpacer {

        }
      }
    }
  }

  Component.onCompleted: {
    console.log(QtOfWidgetNames.names);
  }
}
