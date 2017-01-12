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
                 
