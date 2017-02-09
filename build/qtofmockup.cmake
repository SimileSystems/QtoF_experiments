# -------------------------------------------------------------

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(Qt5Core          REQUIRED)
find_package(Qt5Network       REQUIRED)
find_package(Qt5Widgets       REQUIRED)
find_package(Qt5Gui           REQUIRED)
find_package(Qt5Qml           REQUIRED)
find_package(Qt5Quick         REQUIRED)

list(APPEND qtof_mockup_libs
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  Qt5::Qml
  Qt5::Quick
  )

if (Qt5Core_NOTFOUND)
  message(FATAL_ERROR "Qt5Core not found, make sure to set QT_PATH to the Qt install directory (including e.g. clang_64).")
endif()

list(APPEND qtof_mockup_sources
  ${sd}/qtof/mockup/QtOfExternal.cpp
  ${sd}/qtof/mockup/QtOfExternalWidget.cpp
  ${sd}/qtof/mockup/QtOfPlugin.cpp
  ${sd}/qtof/QtUiMessage.cpp
  ${sd}/qtof/QtWidgetType.cpp
  ${sd}/qml/resources.qrc
  )

add_library(qtof_mockup SHARED ${qtof_mockup_sources})
target_link_libraries(qtof_mockup ${qtof_mockup_libs})

# -------------------------------------------------------------

if (TRUE)
add_custom_target(copy_qtof_mockup
  ALL
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/plugins/qtof/
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:qtof_mockup> ${CMAKE_CURRENT_BINARY_DIR}/plugins/qtof/
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${sd}/qtof/mockup/imports ${CMAKE_CURRENT_BINARY_DIR}/plugins/qtof/
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${sd}/qtof/mockup/designer ${CMAKE_CURRENT_BINARY_DIR}/plugins/qtof/designer
  COMMAND ${QT_PATH}/bin/qmlplugindump -dependencies ${sd}/qtof/mockup/imports/dependency.json -noforceqtquick -nonrelocatable qtof 1.0 ${CMAKE_CURRENT_BINARY_DIR}/plugins/ > ${CMAKE_CURRENT_BINARY_DIR}/plugins/qtof/plugins.qmltypes 
  COMMENT "Copying qtof_mockup plugin to binary dir destination"
  DEPENDS qtof_mockup
  )

set(QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}/plugins/" CACHE string "" FORCE)
endif()
# -------------------------------------------------------------