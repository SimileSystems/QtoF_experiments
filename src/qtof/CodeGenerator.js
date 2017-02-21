/*

  QT CODE GENERATOR
  ==================
  
  GENERAL INFO:

    This code is used to generate QtUiMessage.{h, cpp} and
    QtWidgetType.{h, cpp}. The `QtUiMessage` is generated to expose
    the GUI events in QML. This means that the values that you add to
    `UiMessageTypes.h` can be used directly in your QML.  Note that we
    do change the names a little bit, as we remove the `UI_MSG_` from
    the #defines in `UiMessageTypes.h` You also have to use
    `QtUiMessage` as a prefix to access the types. We do the same with
    the widget types which are defined in WidgetTypes.h. We also
    remove the `WIDGET_TYPE_` from the defines that you've added to
    `WidgetTypes.h`. This generator will also create a file called
    `QtOfWidgetNames.js` that is used by the property inspector in `Qt
    Quick Designer`. This `QtOfWidgetNames.js` file only contains one
    function that returns an array with widget names. These widget
    names are shown in the selectbox in the inspector. When you add
    a new widget type, you have rebuild your Qt project, then restart
    Qt Creator. 
    
    Here are some examples that show how to use the corresponding
    values in `UiMessageTypes.h` in QML.
    
    UI_MSG_WINDOW_RESIZED         QtUiMessage.WINDOW_RESIZED
    UI_MSG_DIRECTORY_CREATE       QtUiMessage.DIRECTORY_CREATE
    UI_MSG_DIRECTORY_DELETE       QtUiMessage.DIRECTORY_DELETE

    This generator uses nodejs and has been tested with version 7.5.0.

  CREATING NEW MESSAGE TYPES:

    It's recommended that you create a header file in your project
    repository and add your custom message types in that file,
    starting with the number 1000. Values < 1000 will be used by the
    qtof library itself. After you've changed or added some `#defines`
    to your custom message type file (e.g. `MyMessages.h`) you  have
    to generate the `QtUiMessage.h` and `QtUiMessage.cpp` files using:

      `node CodeGenerator.js --message-type-headers UiMessageTypes.h MyMessages.h` 

    After running this code generator recompile your Qt project.

  CREATING NEW WIDGET TYPES:

    The way we create new widget types is similar to how we create
    message types.  It's also recommened that you create your own
    header in which you define the widget types, e.g. `MyWidgets.h`.
    Start numbering your widget types at 1000, values < 1000 are used
    by the qtof library. After you've made changes to your
    `MyWidgets.h` file you have to generate the code that is used by
    Qt:

      `node CodeGenerator.js --widget-type-headers WidgetTypes.h MyWidgets.h`

    After running the Code Generator you have to recompile your Qt project.

*/

var fs = require('fs');

/* ------------------------------------------------------ */

function parse_command_line_arguments() {

  var mode = null;
  var result = {
    message_type_headers: new Array(),
    widget_type_headers: new Array()
  };

  for (var i = 0; i < process.argv.length; ++i) {
    var arg = process.argv[i];
    if (arg == "--message-type-headers") {
      mode = "messages";
      continue;
    }
    else if (arg == "--widget-type-headers") {
      mode = "widgets";
      continue;
    }
    
    if (mode == "messages") {
      result.message_type_headers.push(arg);
    }
    else if (mode == "widgets") {
      result.widget_type_headers.push(arg);
    }
  }

  return result;
};

function get_message_defines_from_file(filepath, outEnumsArray) {

  if (false == (outEnumsArray instanceof Array)) {
    console.error("Given outEnumsArray is not an array.");
    return false;
  }

  if (!filepath || 0 == filepath.length) {
    console.error("Given filepath is invalid.");
    return false;
  }

  try {

    /* Read the file with the ui messages. */
    var data = fs.readFileSync(filepath, 'utf8');
    var lines = data.split("\n");
    var msg_enum = new Array();
    
    for (var i = 0; i < lines.length; ++i) {

      var line = lines[i];
      if (line.substring(0, 15) != "#define UI_MSG_") {
        continue;
      }
      
      /* Remove UI_MSG_ */
      line = line.replace(/\s{2,}/g, ' ');
      var define = line.split(" ")[1];
      var value = line.split(" ")[2];
      var parts = define.split("_");
      parts.splice(0, 2);

      /* Create new name, e.g. WINDOW_RESIZED, MOUSE_MOVE, etc. */
      var enum_name = parts.join("_") +" = " +value;
      outEnumsArray.push(enum_name);
    }

    return true;
  }
  catch (e) {
    console.error("Failed to read: ", filepath);
    return false;
  }

  return false;
};

function get_message_defines_from_files(filepaths, outEnumsArray) {

  if (false == (outEnumsArray instanceof Array)) {
    console.error("Given outEnumsArray is not an array.");
    return false;
  }

  if (false == (filepaths instanceof Array)) {
    console.error("Given filepaths is not an array");
    return false;
  }

  for (var i = 0; i < filepaths.length; ++i) {
    if (false == get_message_defines_from_file(filepaths[i], outEnumsArray)) {
      console.error("Failed to read message types.");
      return false;
    }
  }

  return true;
};

function write_message_enums_to_file(enumsArray) {

  if (false == (enumsArray instanceof Array)) {
    console.error("Given enumsArray is not an array, cannot create message type file.");
    return false;
  }

  if (0 == enumsArray.length) {
    console.error("Given enumsArrays is empty, cannot create message type file.");
    return false;
  }

  var msg_enum_str = "enum UiMsg {\n    " +enumsArray.join(",\n    ") +"\n  };\n";

  /* Create the class for Qt. */
  var msg_class_header = ""
      +"/* Generated by " +__filename +", " +new Date() +" */\n"
      +"\n"
      +"#ifndef QT_UI_MESSAGE_H\n"
      +"#define QT_UI_MESSAGE_H\n"
      +"\n"
      +"#include <QObject>\n"
      +"\n"
      +"class QtUiMessage : public QObject {\n"
      +"  Q_OBJECT\n"
      +"\n"
      +"public:\n"
      +"  QtUiMessage();\n"
      +"\n"
      +"  " +msg_enum_str
      +"\n"
      +"  Q_ENUM(UiMsg);"
      +"\n"
      +"\n"
      +"};\n"
      +"\n"
      +"#endif\n"
      +"";

  var msg_class_source = ""
      +"#include <qtof/QtUiMessage.h>\n"
      +"\n"
      +"QtUiMessage::QtUiMessage():QObject() { } \n"
      +"";

  write_file("QtUiMessage.h", msg_class_header);
  write_file("QtUiMessage.cpp", msg_class_source);

  return true;
};

function get_widget_types_from_file(filepath, outWidgetsArray) {

  if (!filepath || 0 == filepath.length) {
    console.error("Invalid filepath.");
    return false;
  }

  if (false == (outWidgetsArray instanceof Array)) {
    console.error("outWidgetsArray is not an array.");
    return false;
  }

  try {
    
    var data = fs.readFileSync(filepath, 'utf8');
    var lines = data.split("\n");
    
    for (var i = 0; i < lines.length; ++i) {

      var line = lines[i];
      if (line.substring(0, 20) != "#define WIDGET_TYPE_") {
        continue;
      }

      /* Remove WIDGET_TYPE_ */
      line = line.replace(/\s{2,}/g, ' ');
      var define = line.split(" ")[1];
      var value = line.split(" ")[2];
      var parts = define.split("_");
      parts.splice(0, 2);

      /* Create new name, e.g. HISTOGRAM, WEBCAM, 3D, DEPTHKIT, etc. */
      var enum_name = parts.join("_") +" = " +value;
      outWidgetsArray.push(enum_name);
    }

    return true;
  }
  catch (e) {
    console.error("Failed to read: ", filepath);
    console.error(e);
    return false;
  }

  return false;
};

function get_widget_types_from_files(filepaths, outWidgetsArray) {
  
  if (false == (filepaths instanceof Array)) {
    console.error("Invalid filepaths given.");
    return false;
  }

  if (false == (outWidgetsArray instanceof Array)) {
    console.error("outWidgetsArray is not an array.");
    return false;
  }

  for (var i = 0; i < filepaths.length; ++i) {
    if (false == get_widget_types_from_file(filepaths[i], outWidgetsArray)) {
      console.error("Failed to get widget types from: ", filepaths[i]);
      return false;
    }
  }

  return true;
};

function write_widget_enums_to_file(enumsArray) {
  
  if (false == (enumsArray instanceof Array)) {
    console.error("Given enumsArray is not an array.");
    return false;
  }

  if (0 == enumsArray.length) {
    console.error("Given enumsArray is empty, not creating widget types file.");
    return false;
  };

  var msg_enum_str = "enum WidgetType {\n    " +enumsArray.join(",\n    ") +"\n  };\n";

  /* Create the class for Qt. */
  var msg_class_header = ""
      +"/* Generated by " +__filename +", " +new Date() +" */\n"
      +"\n"
      +"#ifndef QT_WIDGET_TYPE_H\n"
      +"#define QT_WIDGET_TYPE_H\n"
      +"\n"
      +"#include <QObject>\n"
      +"\n"
      +"class QtWidgetType : public QObject {\n"
      +"  Q_OBJECT\n"
      +"\n"
      +"public:\n"
      +"  QtWidgetType();\n"
      +"\n"
      +"  " +msg_enum_str
      +"\n"
      +"  Q_ENUM(WidgetType);"
      +"\n"
      +"\n"
      +"};\n"
      +"\n"
      +"#endif\n"
      +"";

  var msg_class_source = ""
      +"#include <qtof/QtWidgetType.h>\n"
      +"\n"
      +"QtWidgetType::QtWidgetType():QObject() { } \n"
      +"";

  write_file("QtWidgetType.h", msg_class_header);
  write_file("QtWidgetType.cpp", msg_class_source);
  
  return true;
};

/*

  This function accepts the array with widgets names and values that
  were extracted by using the `get_widget_types_from_file()`
  function. We expect the array to contain values like: `'NONE = 0',
  'DEBUG = 1000'`. We manipulate the values and create a javascript
  file with a function that return the names as an array. This
  function is used in the `QtOfExternalWidgetSpecifics.qml` file to
  fill the listbox with widget types. We store this function in a
  separate file because we don't want the widget types to be part of
  the qtof repository; they are supposed to be part of the users own
  repository.

*/
function write_widget_names_to_file(widgetsArray) {

  if (false == (widgetsArray instanceof Array)) {
    console.error("Given widgets array is not an array; cannot create the names function.");
    return false;
  }

  if (0 == widgetsArray.length) {
    console.error("Given widgetsArray is empty; cannot create the names function.");
    return false;
  }

  /* Get the name parts. */
  var names = new Array();
  for (var i = 0; i < widgetsArray.length; ++i) {
    var w = widgetsArray[i];
    var parts = w.split(' = ');
    if (2 != parts.length) {
      console.error("Unexpected widget name; not storing in the names function.");
      continue;
    }
    names.push(parts[0]);
  }

  if (0 == names.length) {
    console.error("No widget names found. not creating the names function.");
    return false;
  }

  /* Generate the source code. */
  var names_str = '["' +names.join('","') +'"]';
  var code_str = 'function get_widget_names() { return ' +names_str +'; }';

  write_file('mockup/designer/QtOfWidgetNames.js', code_str);

  return true;
};

/*
  This function generates a C++ function that converts the widget type
  integer value into a std::string. This is used by Qt Quick Designer
  to render the name of the plugin into the Gui element that is shown.
  The generated file is used in the mockup, see
  `qtof/mockup/QtOfExternalWidget.cpp`.

*/
function write_widget_to_string_function_to_file(widgetsArray) {

  if (false == (widgetsArray instanceof Array)) {
    console.error("Given widgets array is not an array; cannot create type to string function.");
    return false;
  }

  if (0 == widgetsArray.length) {
    console.error("Given widgetsArray is empty; cannot create type to string function.");
    return false;
  }

  /* Get the name parts. */
  var cases = new Array();
  for (var i = 0; i < widgetsArray.length; ++i) {
    var w = widgetsArray[i];
    var parts = w.split(' = ');
    if (2 != parts.length) {
      console.error("Unexpected widget name; not storing in the names function.");
      continue;
    }
    cases.push("    case QtWidgetType::" +parts[0] +': { return "' +parts[0] +'"; }');
  }
  
  var code = ""
    +"/* Generated by " +__filename +", " +new Date() +" */\n"
    +"#ifndef WIDGET_TYPE_TO_STRING_H\n"
    +"#define WIDGET_TYPE_TO_STRING_H\n"
    +"#include <string>\n"
    +"#include <qtof/QtWidgetType.h>\n"
    +"\n"
    +"inline std::string widget_type_to_string(int type) {\n"
    +"  switch(type) {\n"
    + cases.join("\n") +"\n"
    + '    default: { return "UNKNOWN"; }' +"\n"
    +"  }\n"
    +"}\n\n"
    +"#endif\n"

  write_file('WidgetTypeUtils.h', code);

  return true;
};

function write_file(filename, source) {
  fs.writeFile(filename, source, function(err) {
    if(err) {
      console.error("Failed to write " +filename +".");
      return false;
    }
    console.log(filename +" created.");
  }); 
};

/* ------------------------------------------------------ */

var options = parse_command_line_arguments();

/* Create the widget types files. */
if (0 != options.widget_type_headers.length) {
  var widget_types = new Array();
  if (true == get_widget_types_from_files(options.widget_type_headers, widget_types)) {
    write_widget_enums_to_file(widget_types);
    write_widget_names_to_file(widget_types);
    write_widget_to_string_function_to_file(widget_types);
  }
}

/* Create the message types files. */
if (0 != options.message_type_headers.length) {
  var msg_types = new Array();
  if (true == get_message_defines_from_files(options.message_type_headers, msg_types)) {
    write_message_enums_to_file(msg_types);
  }
}

