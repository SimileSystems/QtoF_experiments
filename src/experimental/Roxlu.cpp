#include <experimental/Roxlu.h>

void Roxlu::setup() {
}

void Roxlu::update() {
  ofExternalWidget::notifyUiMessages();
}

void Roxlu::draw() {
}

void Roxlu::destroy() {
}

void Roxlu::onUiMessage(const UiMessage& msg) {
  switch (msg.type) {
    default: {
      ofExternalWidget::onUiMessage(msg);
      break;
    }
  }
}


