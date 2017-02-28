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
          model: WidgetNames.get_widget_names()
          backendValue: backendValues.widget
          scope: "QtWidgetType"
        }

        ExpandingSpacer {
        }
      }
    }
  }
}
