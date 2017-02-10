import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Rectangle {

  width: 100
  height: 100
  color: "blue"
  property int someVariable;
  property int gl_minor_version
  property int gl_major_version
  
  Component.onCompleted: {
    console.log("loaded from QML_DESIGNER_IMPORT_PATH")
  }
}
