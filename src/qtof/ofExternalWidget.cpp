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

void ofExternalWidget::onUiMessage(const UiMessage& msg) {
  
  switch (msg.type) {

    case UI_MSG_SIZE_CHANGED: {
      widget_width = msg.i[0];
      widget_height = msg.i[1];
      break;
    }
      
    case UI_MSG_POSITION_CHANGED: {
      widget_x = msg.i[0];
      widget_y = msg.i[1];
      break;
    }

    case UI_MSG_PIXEL_RATIO_CHANGED: {
      widget_pixel_ratio = msg.f[0];
      break;
    }

    case UI_MSG_MOUSE_ENTER: {
      widget_is_mouse_inside = true;
      break;
    }
      
    case UI_MSG_MOUSE_LEAVE: {
      widget_is_mouse_inside = false;
      break;
    }

    case UI_MSG_KEY_PRESS: {
      break;
    }

    case UI_MSG_KEY_RELEASE: {
      break;
    }

    default: {
      break;
    }
  }
}
                 
