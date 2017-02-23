if (NOT QT_PATH)
  message(FATAL_ERROR "QT_PATH not set. Set QT_PATH to your Qt base install dir, e.g. /Qt/5.8/clang_64")
endif()

set(CMAKE_PREFIX_PATH ${QT_PATH} CACHE string "" FORCE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(qtof_bd ${CMAKE_CURRENT_LIST_DIR}/..)
set(qtof_sd ${qtof_bd}/src/)

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

if (CMAKE_BUILD_TYPE MATCHES Debug)
  foreach(qtof_lib ${qtof_libs})
    get_target_property(qtof_lib_dll ${qtof_lib} LOCATION_Debug)
    list(APPEND qtof_dlls ${qtof_lib_dll})
  endforeach()
endif()

list(APPEND qtof_dlls
  $<TARGET_FILE:Qt5::Widgets>
  $<TARGET_FILE:Qt5::Core>
  $<TARGET_FILE:Qt5::Gui>
  $<TARGET_FILE:Qt5::Qml>
  $<TARGET_FILE:Qt5::Quick>
  $<TARGET_FILE:Qt5::Network>
  )

find_program(node_js node)
if (NOT node_js)
  message(FATAL_ERROR "Failed to find nodejs. Did you install nodejs and set the PATH variable to the executable directory?")
endif()

include_directories(
  ${qtof_sd}
)

list(APPEND qtof_sources
  ${qtof_sd}/qtof/ofAppExternalWindow.cpp
  ${qtof_sd}/qtof/ofExternal.cpp
  ${qtof_sd}/qtof/ofExternalWidget.cpp
  ${qtof_sd}/qtof/QtOfExternal.cpp
  ${qtof_sd}/qtof/QtOfExternalWidget.cpp
  ${qtof_sd}/qtof/QtUiMessage.cpp
  ${qtof_sd}/qtof/QtWidgetType.cpp
  ${qtof_sd}/qtof/Widgets.cpp
  ${qtof_sd}/qtof/UiMessages.cpp
)

if (NOT EXISTS ${qtof_sd}/qtof/QtUiMessage.cpp)
  set_source_files_properties(${qtof_sd}/qtof/QtUiMessage.cpp PROPERTIES GENERATED TRUE)
endif()

if (NOT EXISTS ${qtof_sd}/qtof/QtWidgetType.cpp)
  set_source_files_properties(${qtof_sd}/qtof/QtWidgetType.cpp PROPERTIES GENERATED TRUE)
endif()

# ------------------------------------------------

macro(qtof_application_create appName appSources)

  add_executable(${appName} ${appSources})
  target_link_libraries(${appName} qtof${debug_flag} ${qtof_libs})
  install(TARGETS ${appName} DESTINATION bin/${appName}/)
  install(FILES ${qtof_dlls} DESTINATION bin/${appName}/)

  foreach(qtof_dll ${qtof_dlls})
    string(MD5 target ${qtof_dll})
    add_custom_target(${target}
      ALL
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qtof_dll} $<TARGET_FILE_DIR:${appName}>
      COMMENT "Copying ${qtof_dll}"
      )
  endforeach()

endmacro()

macro(qtof_widget_create widgetName widgetSources)
  add_library(${widgetName} STATIC ${widgetSources})
  install(TARGETS ${widgetName} DESTINATION lib)
endmacro()

macro(qtof_generate_message_types customMessageTypesHeaderFile)
  add_custom_target(
    generate_message_types
    ALL
    COMMAND ${node_js} CodeGenerator.js --message-type-headers UiMessageTypes.h ${customMessageTypesHeaderFile}
    WORKING_DIRECTORY ${qtof_sd}/qtof
    COMMENT "Generating Qt message type header and source."
    )
endmacro()

macro(qtof_generate_widget_types customWidgetTypesHeaderFile)
  add_custom_target(
    generate_widget_types
    ALL
    COMMAND ${node_js} CodeGenerator.js --widget-type-headers WidgetTypes.h ${customWidgetTypesHeaderFile}
    WORKING_DIRECTORY ${qtof_sd}/qtof
    COMMENT "Generating Qt widget type header and source."
    )
endmacro()

# ------------------------------------------------

add_library(qtof${debug_flag} STATIC ${qtof_sources})
target_link_libraries(qtof${debug_flag} of ${qtof_libs})
install(TARGETS qtof${debug_flag} DESTINATION lib)

