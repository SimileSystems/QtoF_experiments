if (NOT QT_PATH)
  message(FATAL_ERROR "QT_PATH not set. Set QT_PATH to your Qt base install dir, e.g. /Qt/5.8/clang_64")
endif()

set(CMAKE_PREFIX_PATH ${QT_PATH} CACHE string "" FORCE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(qtof_bd ${CMAKE_CURRENT_LIST_DIR}/..)
set(qtof_sd ${qtof_bd}/src/)

include(${CMAKE_CURRENT_LIST_DIR}/of.cmake)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(debug_flag "_debug")
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

if (Qt5Core_NOTFOUND)
  message(FATAL_ERROR "Qt5Core not found. Make sure that you've set the QT_PATH variable to your Qt install directory; e.g. /Applications/Qt/5.8/clang_64")
endif()

list(APPEND qtof_libs
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  Qt5::Qml
  Qt5::Quick
  )

include_directories(
  ${qtof_sd}
)

list(APPEND qtof_sources
  ${qtof_sd}/qtof/ofAppExternalWindow.cpp
  ${qtof_sd}/qtof/ofExternal.cpp
  ${qtof_sd}/qtof/ofExternalWidget.cpp
  ${qtof_sd}/qtof/QtOfExternal.cpp
  ${qtof_sd}/qtof/QtOfExternalWidget.cpp
#  ${qtof_sd}/qtof/QtOfExternalPlugin.cpp
  ${qtof_sd}/qtof/QtUiMessage.cpp
  ${qtof_sd}/qtof/QtWidgetType.cpp
  ${qtof_sd}/qtof/Widgets.cpp
  ${qtof_sd}/qtof/UiMessages.cpp
)

add_library(qtof${debug_flag} STATIC ${qtof_sources})
target_link_libraries(qtof${debug_flag} of ${qtof_libs})
install(TARGETS qtof${debug_flag} DESTINATION lib)

# ------------------------------------------------
macro(qtof_application_create appName appSources)
  add_executable(${appName} ${appSources})
  target_link_libraries(${appName} qtof${debug_flag} ${qtof_libs})
  install(TARGETS ${appName} DESTINATION bin/${appName}/)
endmacro()

macro(qtof_widget_create widgetName widgetSources)
  add_library(${widgetName} STATIC ${widgetSources})
  install(TARGETS ${widgetName} DESTINATION lib)
#  target_link_libraries(${widgetName} qtof${debug_flag})
endmacro()

# ------------------------------------------------