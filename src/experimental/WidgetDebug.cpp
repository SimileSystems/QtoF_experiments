#include <experimental/WidgetDebug.h>

void WidgetDebug::setup() {
}

void WidgetDebug::update() {
  ofExternalWidget::notifyUiMessages();
}

void WidgetDebug::draw() {
}

void WidgetDebug::destroy() {
}

void WidgetDebug::onUiMessage(const UiMessage& msg) {
  switch (msg.type) {
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
  }
}


