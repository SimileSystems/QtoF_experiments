#include <stdio.h>
#include <qtof/ofExternal.h>
#include <qtof/ofExternalWidget.h>

ofExternalWidget::ofExternalWidget()
  :widget_width(0)
  ,widget_height(0)
  ,widget_x(0)
  ,widget_y(0)
  ,widget_pixel_ratio(1.0f)
  ,widget_is_mouse_inside(false)
{
}

int ofExternalWidget::setUiMessageListener(UiMessagesListener* lis) {
  
  if (nullptr == lis) {
    printf("Error: ofExternalWidget::setUiMessageListener(): given listener is nullptr.\n");
    return -1;
  }

  return widget_messages.setListener(lis);
}

void ofExternalWidget::onExternalEvent(const ofExternalEvent& ev) {

  switch (ev.type) {
    case OF_EXT_EVENT_SIZE_CHANGED: {
      widget_width = ev.val.xy[0];
      widget_height = ev.val.xy[1];
      break;
    }
    case OF_EXT_EVENT_POSITION_CHANGED: {
      widget_x = ev.val.xy[0];
      widget_y = ev.val.xy[1];
      break;
    }
    case OF_EXT_EVENT_PIXEL_RATIO_CHANGED: {
      widget_pixel_ratio = ev.val.f;
      break;
    }
    case OF_EXT_EVENT_MOUSE_ENTER: {
      widget_is_mouse_inside = true;
      break;
    }
    case OF_EXT_EVENT_MOUSE_LEAVE: {
      widget_is_mouse_inside = false;
      break;
    }
    default: {
      break;
    }
  }
}

void ofExternalWidget::onUiMessage(const UiMessage& msg) {
  
  printf("ofExternalWidget::onUiMessage(): %d\n", msg.type);

  switch (msg.type) {

    case UI_MSG_SIZE_CHANGED: {
      printf("UI_MSG_SIZE_CHANGED: %d x %d\n", msg.i[0], msg.i[1]);
      widget_width = msg.i[0];
      widget_height = msg.i[1];
      break;
    }
      
    case UI_MSG_POSITION_CHANGED: {
      printf("UI_MSG_POSITION_CHANGED: %d x %d\n", msg.i[0], msg.i[1]);
      widget_x = msg.i[0];
      widget_y = msg.i[1];
      break;
    }

    case UI_MSG_PIXEL_RATIO_CHANGED: {
      printf("UI_MSG_PIXEL_RATIO_CHANGED: %f\n", msg.f[0]);
      widget_pixel_ratio = msg.f[0];
      break;
    }

    case UI_MSG_MOUSE_ENTER: {
      widget_is_mouse_inside = true;
      printf("UI_MSG_MOUSE_ENTER\n");
      break;
    }
      
    case UI_MSG_MOUSE_LEAVE: {
      widget_is_mouse_inside = false;
      printf("UI_MSG_MOUSE_LEAVE\n");
      break;
    }

    case UI_MSG_KEY_PRESS: {
      printf("UI_MSG_KEY_PRESS\n");
      break;
    }

    case UI_MSG_KEY_RELEASE: {
      printf("UI_MSG_KEy_RELEASE\n");
      break;
    }

    default: {
      break;
    }
  }
}
                 
