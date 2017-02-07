cmake_minimum_required(VERSION 3.7.1)
project(Depthkit)
include(ExternalProject)
include(of.cmake)

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
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # This will make sure that the installed qt dylibs are found. 

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
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
  )

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

if (Qt5Core_NOTFOUND)
  message(FATAL_ERROR "Qt5Core not found. Mae sure that you've set the QT_PATH variable to your Qt install directory; e.g. /Applications/Qt/5.8/clang_64")
endif()

list(APPEND qtof_libs
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  Qt5::Qml
  Qt5::Quick
  )

if (WIN32)
  list(APPEND app_libs
    ${ed}/lib/vs2015/libuv.lib
    )
elseif (APPLE)
  list(APPEND app_libs
    ${ed}/lib/clang/libuv.a
    )
endif()

add_library(of${debug_flag} STATIC ${of_sources})
add_library(widgets${debug_flag} STATIC ${widget_sources})
add_library(qtof${debug_flag} SHARED ${qtof_sources})
target_link_libraries(qtof${debug_flag} of${debug_flag} widgets${debug_flag} ${of_libs} ${qtof_libs})

macro(install_qtof_for_target targetName)

  add_dependencies(${targetName} qtof${debug_flag})
  
  add_custom_target(create_plugin_dir
    ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${targetName}>/plugins/qtof/"
    COMMENT "Creating plugin directory `plugins/qtof/` for target `${targetName}`"
    )

  add_custom_target(copy_plugin_lib
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:qtof${debug_flag}> "$<TARGET_FILE_DIR:${targetName}>/plugins/qtof/"
    COMMENT "Copying the qtof${debug_flag} into the plugin directory for target `${targetName}`"
    DEPENDS create_plugin_dir
    )

  add_custom_target(copy_imports
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${sd}/qtof/imports $<TARGET_FILE_DIR:${targetName}>/plugins/qtof/
    COMMENT "Copying the imports directory from `qtof` into target plugins directory for target `${targetName}`."
    DEPENDS create_plugin_dir
    )

  add_custom_target(copy_designer
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${sd}/qtof/designer $<TARGET_FILE_DIR:${targetName}>/plugins/qtof/designer
    COMMENT "Copying designer files"
    )

  # Make sure the rpath is set correctly
  set_target_properties(${targetName} PROPERTIES INSTALL_RPATH "@executable_path/plugins/qtof")

  if (TRUE)
    #COMMAND ${QT_PATH}/bin/qmlplugindump -noforceqtquick -nonrelocatable qtof 1.0 $<TARGET_FILE_DIR:${targetName}>/plugins > $<TARGET_FILE_DIR:${targetName}>/plugins/qtof/plugins.qmltypes 
    add_custom_target(create_qmltypes
      ALL
      COMMAND ${QT_PATH}/bin/qmlplugindump -dependencies ${sd}/qtof/imports/dependency.json -noforceqtquick -nonrelocatable qtof 1.0 $<TARGET_FILE_DIR:${targetName}>/plugins > $<TARGET_FILE_DIR:${targetName}>/plugins/qtof/plugins.qmltypes 
      COMMENT "Executing qmlplugindump"
      DEPENDS copy_imports copy_plugin_lib
      )
  endif()


  
  # Fix for fmodex
  #add_custom_target(fix_fmod
  #  ALL
  #  COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change "@executable_path/libfmodex.dylib" "$<TARGET_FILE_DIR:${targetName}>/libfmodex.dylib" $<TARGET_FILE:qtof${debug_flag}>
  #  COMMENT "Fixing fmod dylib install name"
  #  DEPENDS ${targetName}
  #  )

  set(QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}/test_qt.app/Contents/MacOS/plugins/" CACHE string "" FORCE)

endmacro()

macro(create_test name)

  set(test_name "test_${name}${debug_flag}")
  add_executable(${test_name} MACOSX_BUNDLE ${sd}/test/test_${name}.cpp ${sd}/qml/resources.qrc)
  #target_link_libraries(${test_name} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${of_libs} qtof${debug_flag} ${libs} ${qtof_libs})
  target_link_libraries(${test_name} qtof${debug_flag} ${app_libs})
  install(TARGETS ${test_name} DESTINATION bin)
  install_qtof_for_target(${test_name})
  install_of_for_target(${test_name})
  add_custom_command(TARGET ${test_name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${test_name}>/scenes)

#  qtof_install_for_target(${test_name} TestQmlWidget)
#  of_install_for_target(${test_name})

#  qtof_install_plugin_for_target(${test_name})

endmacro()

create_test(qt)