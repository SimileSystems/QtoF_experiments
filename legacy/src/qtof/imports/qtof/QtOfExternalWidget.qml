import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Item {
  
  property int ref;
  property int type;
  
  Component.onCompleted: {
    console.log("loaded from QML_DESIGNER_IMPORT_PATH")
  }
  
}
