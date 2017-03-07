/*

  WIDGET CODE GENERATOR
  =====================

  GENERAL INFO:

    This code is used to generate some stub code for your widget
    classes.  The generated code can be used as an starting point when
    you want to create your own widgets.

    We will generate 4 files for each widget. Two files called
    [name].h, [name].cpp which are the files that will contain the
    actual source for the features you want to provide. Then we create
    two other files [name]Pimpl.h and [name]Pimpl.cpp which are thin
    opaque wrappers around your actual code that is used by Qt.

  USAGE: 

    Make sure that you've installed nodejs, then open a terminal
    and change into the directory of this file. The execute the 
    following command:
    
    ````sh
      nodejs WidgetCodeGenerator.js --name [NameOfWidget]
    ````

*/

var widget_name = null;
var widget_pimpl_name = null;
var widget_class_name = null;
var widget_pimpl_class_name = null;

/* -------------------------------------------------------------- */

function parse_command_line_arguments() {
  
  var mode = null;
  var result = {
    name: null
  };

  for (var i = 0; i < process.argv.length; ++i) {
    var arg = process.argv[i];
    if (arg == "--name") {
      mode = "name";
      continue;
    }
    if (mode == "name") {
      result.name = arg;
    }
  }

  return result;
};

var opt = parse_command_line_arguments();
if (null == opt.name) {
  console.error("No name given. Usage: node WidgetCodeGenerator.js --name [YourWidgetName]");
  process.exit();
}

widget_name = opt.name;
widget_pimpl_name = widget_name +"Pimpl"
widget_class_name = "Widget" +widget_name;
widget_pimpl_class_name = widget_class_name +"Pimpl";

/* -------------------------------------------------------------- */

var widget_header = ""
  +"#ifndef " +get_header_guard(widget_class_name) +"\n"
  +"#define " +get_header_guard(widget_class_name) +"\n"
  +"\n"
  +"#include <qtof/ofExternalWidget.h>\n"
  +"\n"
  +"class UiMessage;\n"
  +"\n"
  +"class " +widget_class_name +" : public ofExternalWidget {\n"
  +"public:\n"
  +"  void setup();\n"
  +"  void update();\n"
  +"  void draw();\n"
  +"  void destroy();\n"
  +"  void onUiMessage(const UiMessage& msg);\n"
  +"};\n"
  +"\n"
  +"#endif\n"
  +"";

var widget_source = ""
  +"#include <widgets/" +widget_name.toLowerCase() +"/" +widget_class_name +".h>\n"
  +"\n"
  +"void " +widget_class_name +"::setup() {\n"
  +"}\n"
  +"\n"
  +"void " +widget_class_name +"::update() {\n"
  +"  ofExternalWidget::notifyUiMessages();\n"
  +"}\n"
  +"\n"
  +"void " +widget_class_name +"::draw() {\n"
  +"}\n"
  +"\n"
  +"void " +widget_class_name +"::destroy() {\n"
  +"}\n"
  +"\n"
  +"void " +widget_class_name +"::onUiMessage(const UiMessage& msg) {\n"
  +"  switch (msg.type) {\n"
  +"    default: {\n"
  +"      ofExternalWidget::onUiMessage(msg);\n"
  +"      break;\n"
  +"    }\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"\n"
  +"";

var widget_pimpl_header = ""
  +"#ifndef " +get_header_guard(widget_pimpl_class_name) +"\n"
  +"#define " +get_header_guard(widget_pimpl_class_name) +"\n"
  +"\n"
  +"class " +widget_class_name +";\n"
  +"class UiMessage;\n"
  +"class UiMessagesListener;\n"
  +"\n"
  +"class " +widget_pimpl_class_name +" {\n"
  +"public:\n"
  +"  " +widget_pimpl_class_name +"();\n"
  +"  void setup();\n"
  +"  void update();\n"
  +"  void draw();\n"
  +"  void destroy();\n"
  +"  void onUiMessage(const UiMessage& msg);\n"
  +"  void setUiMessageListener(UiMessagesListener* lis);\n"
  +"\n"
  +"private:\n"
  +"  " +widget_class_name +"* ctx;\n"
  +"};\n"
  +"\n"
  +"#endif\n"
  +"";

var widget_pimpl_source = ""
  +"#include <widgets/" +widget_name.toLowerCase() +"/" +widget_class_name +".h>\n"
  +"#include <widgets/" +widget_name.toLowerCase() +"/" +widget_pimpl_class_name +".h>\n"
  +"\n"
  +widget_pimpl_class_name +"::" +widget_pimpl_class_name +"() \n"
  +":ctx(nullptr)\n"
  +"{\n"
  +"  ctx = new " +widget_class_name +"();\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_class_name +"::setup() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->setup();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_class_name +"::update() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->update();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_class_name +"::draw() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->draw();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_class_name +"::destroy() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->destroy();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_class_name +"::onUiMessage(const UiMessage& msg) {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->onUiMessage(msg);\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_class_name +"::setUiMessageListener(UiMessagesListener* lis) {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->setUiMessageListener(lis);\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"";

console.log(widget_header +"------------------------\n\n");
console.log(widget_source +"------------------------\n\n");
console.log(widget_pimpl_header +"------------------------\n\n");
console.log(widget_pimpl_source +"------------------------\n\n");

write_file(widget_class_name +".h", widget_header);
write_file(widget_class_name +".cpp", widget_source);
write_file(widget_pimpl_class_name +".h", widget_pimpl_header);
write_file(widget_pimpl_class_name +".cpp", widget_pimpl_source);

/* -------------------------------------------------------------- */

function get_header_guard(widgetName) {
  
  var name_parts = widgetName.match(/[A-Z][a-z]+/g);
  var header_guard = "";
  
  for (var i = 0; i < name_parts.length; ++i) {
    header_guard += name_parts[i].toUpperCase();
    if ( (i + 1) < name_parts.length) {
      header_guard += "_";
    }
  }
  
  header_guard += "_H";

  return header_guard;
}

function write_file(filename, source) {
  var fs = require('fs');
  fs.writeFile(filename, source, function(err) {
    if(err) {
      console.error("Failed to write " +filename +".");
      return false;
    }
    console.log(filename +" created.");
  }); 
}
