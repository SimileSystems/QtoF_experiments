set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src)
set(ed ${bd}/extern)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT CMAKE_PREFIX_PATH AND NOT DEFINED ENV{QT_PATH})
  message(FATAL_ERROR "Please add an environment variable called QT_PATH that is set to the Qt install directory, like: G:\\applications\\Qt\\5.8\\msvc2015_64")
endif()

if (ENV{QT_PATH})
  set(CMAKE_PREFIX_PATH "$ENV{QT_PATH}")
endif()

find_package(Qt5Core          REQUIRED)
find_package(Qt5Network       REQUIRED)
find_package(Qt5Widgets       REQUIRED)
find_package(Qt5Gui           REQUIRED)
find_package(Qt5Qml           REQUIRED)
find_package(Qt5Quick         REQUIRED)

if (WIN32)
  list(APPEND qtof_assets
    $<TARGET_FILE:Qt5::Widgets>
    $<TARGET_FILE:Qt5::Gui>
    $<TARGET_FILE:Qt5::Qml>
    $<TARGET_FILE:Qt5::Quick>
    $<TARGET_FILE:Qt5::Core>
    $<TARGET_FILE:Qt5::Network>
    )
endif()

include_directories(
  ${sd}
  ${ed}/include
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

