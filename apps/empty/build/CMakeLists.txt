cmake_minimum_required(VERSION 3.7.1)
include(ExternalProject)
project(Depthkit)

set(bd ${CMAKE_CURRENT_LIST_DIR}/../../../)    # Repository base dir.
set(pd ${CMAKE_CURRENT_LIST_DIR}/../)          # Project base dir.
set(ed ${bd}/extern)                           # External resources dir.
set(app_sd ${pd}/src)                          # Project source dir.
set(app_qd ${pd}/qml)                          # QML Directory
set(CMAKE_CXX_STANDARD 11)

include(${bd}/build/of.cmake)
include(${bd}/build/qtof.cmake)

include_directories(
  ${sd}
  ${ed}/include
  )

# Sources for this Application
list(APPEND app_sources
  ${bd}/src/experimental/WidgetDepthKit.cpp
  ${bd}/src/experimental/WidgetDepthKitPimpl.cpp
  ${bd}/src/experimental/DirectoryWatcher.cpp
  )

# Libraries for this Application
list(APPEND app_libs
  ${ed}/lib/libuv.a
  )

# openFrameworks and our Qt wrappers.
add_library(of STATIC ${of_sources})
add_library(qtof STATIC ${qtof_sources})
target_link_libraries(qtof Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick ${of_libs} of ${libs})
add_dependencies(qtof of)

# Application.
add_executable(${PROJECT_NAME} ${app_sd}/main.cpp ${app_sources} ${app_qd}/resources.qrc)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${of_libs} ${app_libs} qtof)
install(TARGETS ${PROJECT_NAME} DESTINATION bin/)

# Custom for this Application
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/scenes)
