cmake_minimum_required(VERSION 3.7.1)
include(ExternalProject)
project(101_qt)
include(of.cmake)
include(qtof.cmake)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(debug_flag "_debug")
endif()

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src)
set(ed ${bd}/extern)
#set(CMAKE_PREFIX_PATH /Applications/Qt/5.7/clang_64/)
#set(CMAKE_PREFIX_PATH "/g/applications/Qt/5.8/msvc2015_64/lib/cmake/")
set(CMAKE_PREFIX_PATH "/g/applications/Qt/5.8/msvc2015_64/")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # This will make sure that the installed qt dylibs are found. 


#find_package(Qt5Widgets       REQUIRED)
#find_package(Qt5Gui           REQUIRED)
#find_package(Qt5Qml           REQUIRED)
#find_package(Qt5Quick         REQUIRED)
#find_package(Qt5QuickCompiler REQUIRED)   # From http://doc.qt.io/QtQuickCompiler/qquickcompiler-building-with-cmake.html
#find_package(Qt5Core       REQUIRED)
#find_package(Qt5Network       REQUIRED)

#message("________ ${Qt5Network_DIR} _______")
#message("________ $<TARGET_FILE:Qt5::Widgets> ___________")

include_directories(
  ${ed}/include
  ${sd}
  )

list(APPEND of_sources
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
  ${sd}/qtof/QtUiMessage.cpp
  ${sd}/qtof/Widgets.cpp
  ${sd}/qtof/UiMessages.cpp
  )

if (WIN32)
  
  list(APPEND libs
    ${ed}/lib/vs2015/libuv.lib
    )

elseif (APPLE)
  list(APPEND libs
    ${ed}/lib/clang/libuv.a
    )
endif()

# openFrameworks library
add_library(of${debug_flag} STATIC ${of_sources})

# QtOf library
add_library(qtof${debug_flag} STATIC ${qtof_sources})
target_link_libraries(qtof${debug_flag} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick of${debug_flag} ${of_libs} ${libs})
add_dependencies(qtof${debug_flag} of${debug_flag})

# tests
macro(create_test name)
  set(test_name "test_${name}${debug_flag}")
  add_executable(${test_name} ${sd}/test/test_${name}.cpp ${sd}/qml/resources.qrc)
  target_link_libraries(${test_name} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${of_libs} qtof${debug_flag} ${libs})
  install(TARGETS ${test_name} DESTINATION bin/)
endmacro()

message(${of_libs})
create_test(qt)

# for the test....
install(DIRECTORY DESTINATION bin/scenes)

#add_executable(test_qt${debug_flag} ${sd}/test_qt.cpp ${sd}/qml/resources.qrc) 
#target_link_libraries(test_qt${debug_flag} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick render${debug_flag})
#install(TARGETS test_qt${debug_flag} DESTINATION bin)
