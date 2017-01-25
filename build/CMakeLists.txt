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

# Helper to create tests
macro(create_test name)
  set(test_name "test_${name}${debug_flag}")
  add_executable(${test_name} MACOSX_BUNDLE ${sd}/test/test_${name}.cpp ${sd}/qml/resources.qrc)
  target_link_libraries(${test_name} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${of_libs} qtof${debug_flag} ${libs})
  install(TARGETS ${test_name} DESTINATION bin/)

  # Create the "scenes" directory (Custom for this project).
  add_custom_target(create_scenes_dir ALL COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${test_name}>/scenes)

  # Install the OF assets. 
  foreach(of_asset ${of_assets}) 
    add_custom_target(install_of_assets ALL COMMAND ${CMAKE_COMMAND} -E copy ${of_asset} $<TARGET_FILE_DIR:${test_name}>)
  endforeach()

endmacro()

create_test(qt)

