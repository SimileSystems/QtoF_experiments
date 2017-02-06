set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src)
set(ed ${bd}/extern)
set(pd ${bd}/src/experimental/) # Plugin base dir.

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


if(NOT CMAKE_PREFIX_PATH AND NOT DEFINED ENV{QT_PATH})
  message(FATAL_ERROR "Please add an environment variable called QT_PATH that is set to the Qt install directory, like: G:\\applications\\Qt\\5.8\\msvc2015_64")
endif()

if (DEFINED ENV{QT_PATH})
  set(CMAKE_PREFIX_PATH "$ENV{QT_PATH}")
endif()

find_package(Qt5Core          REQUIRED)
find_package(Qt5Network       REQUIRED)
find_package(Qt5Widgets       REQUIRED)
find_package(Qt5Gui           REQUIRED)
find_package(Qt5Qml           REQUIRED)
find_package(Qt5Quick         REQUIRED)

message(STATUS "VERSION: ${Qt5Core_VERSION}")

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

  #${sd}/experimental/TestQmlWidgetQtDesigner.cpp
  )

add_library(TestQmlWidget SHARED
  ${sd}/experimental/TestQmlWidget/TestQmlWidget.cpp
  ${sd}/experimental/TestQmlWidget/TestQmlWidgetExtensionPlugin.cpp
  )

target_link_libraries(TestQmlWidget Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)


#list(APPEND qtof_shared_libs_to_install
#  TestQmlWidgetExtensionPlugin
#  )

#list(APPEND qtof_libs
#  TestQmlWidgetExtensionPlugin
#  )

#set(plugin_base_dir "${CMAKE_CURRENT_BINARY_DIR}/plugins")

#set_target_properties(TestQmlWidget PROPERTIES
#  MACOSX_RPATH TRUE
#  LIBRARY_OUTPUT_DIRECTORY "${plugin_base_dir}/roxlu"
#  )

#get_target_property(plugin_library_output_dir TestQmlWidgetExtensionPlugin LIBRARY_OUTPUT_DIRECTORY)
#get_target_property(qtcore_location Qt5::Core LOCATION)
#set(plugin_lib_name "TestQmlWidgetExtensionPlugin")
#set(plugin_module_name "roxlu")

macro(qtof_install_for_target targetName pluginName)

  set_target_properties(${targetName} PROPERTIES INSTALL_RPATH "@executable_path/plugins/roxlu")

  # Create plugin dir
  #add_custom_command(
  #  TARGET ${targetName}
  #  POST_BUILD
  #  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${targetName}>/plugins/roxlu"
  #  )

  # Copy plugin
 # add_custom_command(
 #   TARGET ${targetName}
 #   POST_BUILD
 #   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${pluginName}> "$<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/"
 #   )

  # QmlPluginDump
  #add_custom_command(
  #  TARGET ${targetName}
  #  POST_BUILD
  #  COMMAND qmlplugindump roxlu 1.0 $<TARGET_FILE_DIR:${targetName}>/plugins > $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/plugins.qmltypes
  #  )

  # Create plugins dir
  add_custom_target(create_plugin_dir
    ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${targetName}>/plugins/roxlu"
    COMMENT "Create plugins dir"
    )
  

  # Copy dylib
  add_custom_target(copy_lib
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${pluginName}> "$<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/"
    COMMENT "Copying plugin library"
    )

  # Copy qmldir file
  add_custom_target(copy_qmldir
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${pd}/${pluginName}/qmldir $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/
    COMMENT "Copying qmldir file"
    )

  # Copy Qt Quick Designer files
  add_custom_target(copy_designer
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${pd}/${pluginName}/designer $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/designer
    COMMENT "Copying designer files"
    )

  # Test: copy temp qml
  add_custom_target(copy_qml
    ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${pd}/${pluginName}/TestQmlWidgetWrapper.qml $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/
    COMMENT "Copying test qml file"
    DEPENDS create_plugin_dir
    )

  # Create qmltypes file (Make sure .dylib + qmldir files are there)
  # - Cannot use $ENV{QT_PATH} as it's not available in Qt Creator
  # Use -nonrelocatable because otherwise the "exports" value is wrong, see https://gist.github.com/roxlu/6114e1371143e73ae686775db3a1b338
  if (TRUE)
  add_custom_target(create_qmltypes
    ALL
    COMMAND ${QT_PATH}/bin/qmlplugindump -nonrelocatable roxlu 1.0 $<TARGET_FILE_DIR:${targetName}>/plugins > $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/plugins.qmltypes
    COMMENT "Executing qmlplugindump"
    DEPENDS copy_qmldir copy_lib copy_qml
    )
  endif()
  #add_dependencies(${targetName} ${pluginName})

#  add_custom_command(
#    TARGET ${targetName}
#    POST_BUILD
#    COMMAND qmlplugindump 
#    )

  
#  add_custom_command(
#    TARGET ${targetName}
#    POST_BUILD
#    COMMAND "/Applications/Qt/5.7/clang_64/bin/qmlplugindump roxlu 1.0 $<TARGET_FILE_DIR:${targetName}>/plugins > test.txt"
#    )
  #message("/Applications/Qt/5.7/clang_64/bin/qmlplugindump roxlu 1.0 $<TARGET_FILE_DIR:${targetName}>/plugins > $<TARGET_FILE_DIR:${targetName}>/plugins/plugins.qmltypes")

 
  #message("___ ${pd}/${pluginName}/qmldir ___")
  install(FILES ${pd}/${pluginName}/qmldir DESTINATION $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/)
  install(TARGETS ${pluginName} DESTINATION $<TARGET_FILE_DIR:${targetName}>/plugins/roxlu/)
  
endmacro()

#macro(qtof_create_plugin pluginName pluginSources

