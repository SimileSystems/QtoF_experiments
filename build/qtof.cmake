set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src)
set(ed ${bd}/extern)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (APPLE)
  set(CMAKE_PREFIX_PATH /Applications/Qt/5.7/clang_64/)
endif()

find_package(Qt5Core          REQUIRED)
find_package(Qt5Network       REQUIRED)
find_package(Qt5Widgets       REQUIRED)
find_package(Qt5Gui           REQUIRED)
find_package(Qt5Qml           REQUIRED)
find_package(Qt5Quick         REQUIRED)
#find_package(Qt5QuickCompiler REQUIRED)

install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Gui>     DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Qml>     DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Quick>   DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Core>    DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Network> DESTINATION bin)

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

