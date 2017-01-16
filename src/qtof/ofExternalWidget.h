#ifndef OF_EXTERNAL_WIDGET_H
#define OF_EXTERNAL_WIDGET_H

/* ------------------------------------------------------ */

class ofExternalEvent;

/* ------------------------------------------------------ */

class ofExternalWidget {
public:
  ofExternalWidget();
  virtual void onExternalEvent(const ofExternalEvent& ev);


  /* Mouse */
  bool isMouseInsideWidget(); /* Returns true when the mouse is inside the x/y and width/height of the widget. */
  
  /* Size and position. */
  int getWidgetWidth();
  int getWidgetHeight();
  int getWidgetX();
  int getWidgetY();
  float getPixelRatio();

  /* Pixel ratio corrected. */
  int getWidgetDrawWidth();
  int getWidgetDrawHeight();
  int getWidgetDrawX();
  int getWidgetDrawY();

private:
  int widget_width;
  int widget_height;
  int widget_x;
  int widget_y;
  float widget_pixel_ratio;
  bool widget_is_mouse_inside;
};

/* ------------------------------------------------------ */

inline bool ofExternalWidget::isMouseInsideWidget() {
  return widget_is_mouse_inside;
}

/* ------------------------------------------------------ */

inline int ofExternalWidget::getWidgetWidth() {
  return widget_width;
}

inline int ofExternalWidget::getWidgetHeight() {
  return widget_height;
}

inline int ofExternalWidget::getWidgetX() {
  return widget_x;
}

inline int ofExternalWidget::getWidgetY() {
  return widget_y;
}

inline float ofExternalWidget::getPixelRatio() {
  return widget_pixel_ratio;
}

/* ------------------------------------------------------ */

inline int ofExternalWidget::getWidgetDrawWidth() {
  return widget_width * widget_pixel_ratio;
}

inline int ofExternalWidget::getWidgetDrawHeight() {
  return widget_height * widget_pixel_ratio;
}

inline int ofExternalWidget::getWidgetDrawX() {
  return widget_x * widget_pixel_ratio;
}

inline int ofExternalWidget::getWidgetDrawY() {
  return widget_y * widget_pixel_ratio;
}

/* ------------------------------------------------------ */

#endif
