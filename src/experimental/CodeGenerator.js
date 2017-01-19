var widget_name = "WidgetDebug";
var widget_pimpl_name = widget_name +"Pimpl";

/* -------------------------------------------------------------- */

var widget_header = ""
  +"#ifndef " +get_header_guard(widget_name) +"\n"
  +"#define " +get_header_guard(widget_name) +"\n"
  +"\n"
  +"#include <qtof/ofExternalWidget.h>\n"
  +"\n"
  +"class UiMessage;\n"
  +"\n"
  +"class " +widget_name +" : public ofExternalWidget {\n"
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
  +"#include <experimental/" +widget_name +".h>\n"
  +"\n"
  +"void " +widget_name +"::setup() {\n"
  +"}\n"
  +"\n"
  +"void " +widget_name +"::update() {\n"
  +"  ofExternalWidget::notifyUiMessages();\n"
  +"}\n"
  +"\n"
  +"void " +widget_name +"::draw() {\n"
  +"}\n"
  +"\n"
  +"void " +widget_name +"::destroy() {\n"
  +"}\n"
  +"\n"
  +"void " +widget_name +"::onUiMessage(const UiMessage& msg) {\n"
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
  +"#ifndef " +get_header_guard(widget_pimpl_name) +"\n"
  +"#define " +get_header_guard(widget_pimpl_name) +"\n"
  +"\n"
  +"class " +widget_name +";\n"
  +"class UiMessage;\n"
  +"class UiMessagesListener;\n"
  +"\n"
  +"class " +widget_pimpl_name +" {\n"
  +"public:\n"
  +"  " +widget_pimpl_name +"();\n"
  +"  void setup();\n"
  +"  void update();\n"
  +"  void draw();\n"
  +"  void destroy();\n"
  +"  void onUiMessage(const UiMessage& msg);\n"
  +"  void setUiMessageListener(UiMessagesListener* lis);\n"
  +"\n"
  +"private:\n"
  +"  " +widget_name +"* ctx;\n"
  +"};\n"
  +"\n"
  +"#endif\n"
  +"";

var widget_pimpl_source = ""
  +"#include <experimental/" +widget_name +".h>\n"
  +"#include <experimental/" +widget_pimpl_name +".h>\n"
  +"\n"
  +widget_pimpl_name +"::" +widget_pimpl_name +"() \n"
  +":ctx(nullptr)\n"
  +"{\n"
  +"  ctx = new " +widget_name +"();\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_name +"::setup() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->setup();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_name +"::update() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->update();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_name +"::draw() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->draw();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_name +"::destroy() {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->destroy();\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_name +"::onUiMessage(const UiMessage& msg) {\n"
  +"  if (nullptr != ctx) {\n"
  +"    ctx->onUiMessage(msg);\n"
  +"  }\n"
  +"}\n"
  +"\n"
  +"void " +widget_pimpl_name +"::setUiMessageListener(UiMessagesListener* lis) {\n"
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

write_file(widget_name +".h", widget_header);
write_file(widget_name +".cpp", widget_source);
write_file(widget_pimpl_name +".h", widget_pimpl_header);
write_file(widget_pimpl_name +".cpp", widget_pimpl_source);

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
