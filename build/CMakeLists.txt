cmake_minimum_required(VERSION 3.7.1)
project(Depthkit)
include(ExternalProject)
include(of.cmake)
include(qtofmockup.cmake)

# The user has to specificy the QT_PATH variable which is set to the
# base directory of yor Qt install. E.g.  /Qt/5.8/clang_64. We need
# this path to be able to include the Qt cmake modules. We also use
# this to find the qmlplugindump util that we use to create the
# qmltypes file.
if (NOT QT_PATH)
  message(FATAL_ERROR "QT_PATH not set. Set QT_PATH to your Qt base install dir, e.g. /Qt/5.8/clang_64")
endif()

set(CMAKE_PREFIX_PATH ${QT_PATH} CACHE string "" FORCE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(bd ${CMAKE_CURRENT_LIST_DIR}/..)
set(sd ${bd}/src)
set(ed ${bd}/extern)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(debug_flag "_debug")
endif()

include_directories(
  ${ed}/include
  ${sd}
  )

list(APPEND widget_sources
  ${sd}/experimental/TimWidgetPimpl.cpp
  ${sd}/experimental/TimWidget.cpp
  ${sd}/experimental/WidgetHistogramPimpl.cpp
  ${sd}/experimental/WidgetHistogram.cpp
  ${sd}/experimental/WidgetWebcamPimpl.cpp
  ${sd}/experimental/WidgetWebcam.cpp
  ${sd}/experimental/WidgetDepthKit.cpp
  ${sd}/experimental/WidgetDepthKitPimpl.cpp
  ${sd}/experimental/Widget3d.cpp
  ${sd}/experimental/Widget3dPimpl.cpp
  ${sd}/experimental/WidgetDebug.cpp
  ${sd}/experimental/WidgetDebugPimpl.cpp
  ${sd}/experimental/GuiBlur.cpp
  ${sd}/experimental/DirectoryWatcher.cpp
  )

list(APPEND qtof_sources
  ${ed}/src/glad.c
  ${sd}/qtof/ofAppExternalWindow.cpp
  ${sd}/qtof/ofExternal.cpp
  ${sd}/qtof/ofExternalWidget.cpp
  ${sd}/qtof/QtOfExternal.cpp
  ${sd}/qtof/QtOfExternalWidget.cpp
  ${sd}/qtof/QtOfExternalPlugin.cpp
  ${sd}/qtof/QtUiMessage.cpp
  ${sd}/qtof/QtWidgetType.cpp
  ${sd}/qtof/Widgets.cpp
  ${sd}/qtof/UiMessages.cpp
#  ${sd}/qtof/Register.cpp
  )

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

if (WIN32)
 
  list(APPEND qtof_assets
    $<TARGET_FILE:Qt5::Widgets>
    $<TARGET_FILE:Qt5::Gui>
    $<TARGET_FILE:Qt5::Qml>
    $<TARGET_FILE:Qt5::Quick>
    $<TARGET_FILE:Qt5::Core>
    $<TARGET_FILE:Qt5::Network>
    )
  
  list(APPEND app_libs
    ${ed}/lib/vs2015/libuv.lib
    )
  
elseif (APPLE)
  
  list(APPEND app_libs
    ${ed}/lib/clang/libuv.a
    )
  
endif()

list(APPEND of_sources
  ${ed}/src/glad.c
  )

add_library(of${debug_flag} STATIC ${of_sources})
add_library(widgets${debug_flag} STATIC ${widget_sources})
add_library(qtof${debug_flag} STATIC ${qtof_sources})
target_link_libraries(qtof${debug_flag} ${qtof_libs})

macro(create_test name)
  set(test_name "test_${name}${debug_flag}")
  add_executable(${test_name} MACOSX_BUNDLE ${sd}/test/test_${name}.cpp ${sd}/qml/resources.qrc  ${ed}/src/glad.c)
  target_link_libraries(${test_name} of${debug_flag} qtof${debug_flag} ${app_libs} widgets${debug_flag} ${of_libs})
  install(TARGETS ${test_name} DESTINATION bin)
  install_of_for_target(${test_name})
  add_custom_command(TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${test_name}>/scenes)
  install(DIRECTORY DESTINATION bin/scenes)
endmacro()

create_test(qt)