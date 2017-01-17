#include <QRunnable>
#include <qtof/QtOfExternalWidget.h>
#include <qtof/QtOfWidgets.h>
#include <qtof/ofExternal.h>

/* ---------------------------------------------------- */

class QtOfExternalWidgetCleanupRunnable : public QRunnable {
public:
  QtOfExternalWidgetCleanupRunnable(int ref);
  void run();
  
private:
  int ref;
};

/* ---------------------------------------------------- */

QtOfExternalWidget::QtOfExternalWidget()
  :ref(-1)
  ,is_created(false)
{
  connect(this, &QQuickItem::windowChanged, this, &QtOfExternalWidget::onWindowChanged);
  setAcceptHoverEvents(true);
}

/* 

   When Qt decides to remove this QML item, we have to make sure that
   the OF widget also cleans up any of it's GL resources (textures,
   shaders, etc.). We provide a function `qtof_widget_destroy()` that
   calls `destroy()` on the OF based widget. But we have to make sure
   that this is called from the render thread when there is an GL
   context active. We're scheduling a render job for this purpose as
   described in the [QQuickItem][1] documentation.

   We also have to clear framebuffer because otherwise the contents of
   the widget may be still visible.

*/
QtOfExternalWidget::~QtOfExternalWidget() {

  if (nullptr != window()) {
    window()->setClearBeforeRendering(true); 
    window()->scheduleRenderJob(new QtOfExternalWidgetCleanupRunnable(ref), QQuickWindow::BeforeSynchronizingStage);
  }

  ref = -1;
  is_created = false;
}

/*

  This function is called by the widget (e.g. the openFrameworks
  widget). This function will be called when the widget calls the
  `notify()` function of the `UiMessages` member. A widget should call
  `UiMessages::notify()` from it's `update()` function, or when the 
  widget extends `ofExternalWidget` it can call `notifyUiMessages()`.
  
 */
void QtOfExternalWidget::onUiMessage(const UiMessage& msg) {
  printf("QtOfExternalWidget::onUiMessage() - test onEvent call.\n");

  QVariantMap js_msg;
  js_msg["type"] = msg.type;
  js_msg["s"] = QString(msg.s.c_str());

  QVariantList int_list;
  int_list.insert(0, msg.i[0]);
  int_list.insert(1, msg.i[1]);
  int_list.insert(2, msg.i[2]);
  int_list.insert(3, msg.i[3]);
  js_msg["i"] = int_list;

  QVariantList float_list;
  float_list.insert(0, msg.i[0]);
  float_list.insert(1, msg.i[1]);
  float_list.insert(2, msg.i[2]);
  float_list.insert(3, msg.i[3]);
  js_msg["f"] = float_list;
  
  QMetaObject::invokeMethod(this, "onUiMessage", Qt::DirectConnection, Q_ARG(QVariant, QVariant::fromValue(js_msg)));
}

void QtOfExternalWidget::onSync() {
  
  if (false == is_created) {
    connect(window(), &QQuickWindow::beforeRendering, this, &QtOfExternalWidget::onPaint, Qt::DirectConnection);
    window()->update();
  }
}

void QtOfExternalWidget::onCleanup() {
  qDebug() << "onCleanup";
}

void QtOfExternalWidget::onPaint() {

  static int paint_count = 0;
  
  if (-1 == ref) {
    qWarning() << "QtOfExternalWidget::onPaint() being called but `ref` is -1. Not supposed to happen.";
    return;
  }
  
  if (false == is_created) {

    if (0 != qtof_widget_create(ref)) {
      qFatal("Failed to create the QtOfExternalWidget.");
      return;
    }

    qtof_widget_set_message_listener(ref, this);

    /* Notify some info that's needed when setting up. */
    notifySize();
    notifyPixelRatio();
    notifyPosition();
    qtof_widget_update(ref);


    if (0 != qtof_widget_setup(ref)) {
      /* @todo destroy the created factory. */
      qFatal("Failed to setup the widgte for reference: %d", ref);
      return;
    }

    resetOpenGlState();
    
    is_created = true;
  }

  if (0 == paint_count) {
    of_external_start_render();
  }

  qtof_widget_update(ref);
  qtof_widget_draw(ref);

  paint_count++;
  if (paint_count >= qtof_widget_get_num_widgets()) {
    paint_count = 0;
    of_external_finish_render();
  }

  resetOpenGlState();
}

/* ---------------------------------------------------- */

/*

  This slot is signalled when the window has been created. At this
  point we connect our slots to the signals from the QQuickWindow. We
  are using the Qt::DirectConnection because we want our slots to be
  signalled from the render-thread; not the GUI-thread.

 */
void QtOfExternalWidget::onWindowChanged(QQuickWindow* win) {

  if (nullptr == win) {
    qDebug() << "Not initializing the QtOfExternalWidget; win is null.";
    return;
  }

  connect(win, &QQuickWindow::beforeSynchronizing, this, &QtOfExternalWidget::onSync, Qt::DirectConnection);
  connect(win, &QQuickWindow::sceneGraphInvalidated, this, &QtOfExternalWidget::onCleanup, Qt::DirectConnection);
  connect(win, &QQuickWindow::screenChanged, this, &QtOfExternalWidget::onScreenChanged);

  /* @todo do we want to call this here? */
  win->setClearBeforeRendering(false);
}

void QtOfExternalWidget::onScreenChanged(QScreen* screen) {
  notifyPixelRatio();
}

/* ---------------------------------------------------- */

void QtOfExternalWidget::notifySize() {
  /*
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_SIZE_CHANGED;
  ev.val.xy[0] = width();
  ev.val.xy[1] = height();
  qtof_widget_send_event(ref, ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_SIZE_CHANGED;
  msg.i[0] = width();
  msg.i[1] = height();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::notifyPosition() {
  /*
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_POSITION_CHANGED;
  ev.val.xy[0] = x();
  ev.val.xy[1] = y();
  qtof_widget_send_event(ref, ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_POSITION_CHANGED;
  msg.i[0] = x();
  msg.i[1] = y();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::notifyPixelRatio() {
  /*
  ofExternalEvent ev;
  ev.type = OF_EXT_EVENT_PIXEL_RATIO_CHANGED;
  ev.val.f = window()->devicePixelRatio();
  qtof_widget_send_event(ref, ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_PIXEL_RATIO_CHANGED;
  msg.f[0] = window()->devicePixelRatio();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::resetOpenGlState() {
  window()->resetOpenGLState();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

/* ---------------------------------------------------- */

/*
  @todo atm I'm not sure how to handle this situation. When 
  I would remove the widget (qtof_widget_destroy) this results
  in a crash because `onPaint` is still being called. 

  You can test this by setting the parent of the QML node 
  to null (in your QML create a button for example).

 */

void QtOfExternalWidget::releaseResources() {
  qDebug() << "releaseResources!";
  /*
  window()->scheduleRenderJob(new QtOfExternalWidgetCleanupRunnable(ref), QQuickWindow::AfterRenderingStage); // BeforeSynchronizingStage);
  ref = -1;
  QQuickItem::releaseResources();
  */
}

void QtOfExternalWidget::hoverEnterEvent(QHoverEvent* ev) {
  /*
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_MOUSE_ENTER;
  ext_ev.val.mouse[0] = ev->pos().x();
  ext_ev.val.mouse[1] = ev->pos().y();
  qtof_widget_send_event(ref, ext_ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_MOUSE_ENTER;
  msg.i[0] = ev->pos().x();
  msg.i[1] = ev->pos().y();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::hoverLeaveEvent(QHoverEvent* ev) {
  /*
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_MOUSE_LEAVE;
  ext_ev.val.mouse[0] = ev->pos().x();
  ext_ev.val.mouse[1] = ev->pos().y();
  qtof_widget_send_event(ref, ext_ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_MOUSE_LEAVE;
  msg.i[0] = ev->pos().x();
  msg.i[1] = ev->pos().y();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::keyPressEvent(QKeyEvent* ev) {
  /*
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_KEY_PRESS;
  ext_ev.val.i = ev->key();
  qtof_widget_send_event(ref, ext_ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_KEY_PRESS;
  msg.i[0] = ev->key();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::keyReleaseEvent(QKeyEvent* ev) {
  /*
  ofExternalEvent ext_ev;
  ext_ev.type = OF_EXT_EVENT_KEY_RELEASE;
  ext_ev.val.i = ev->key();
  qtof_widget_send_event(ref, ext_ev);
  */

  UiMessage msg;
  msg.type = UI_MSG_KEY_RELEASE;
  msg.i[0] = ev->key();
  qtof_widget_send_message(ref, msg);
}

void QtOfExternalWidget::sendExternalEventFloat(uint32_t eventType, const float& v) {
  ofExternalEvent ext_ev;
  ext_ev.type = eventType;
  ext_ev.val.f = v;
  qtof_widget_send_event(ref, ext_ev);
}

void QtOfExternalWidget::sendExternalEventInt(uint32_t eventType, const int& v) {
  ofExternalEvent ext_ev;
  ext_ev.type = eventType;
  ext_ev.val.i = v;
  qtof_widget_send_event(ref, ext_ev);
}

/*

  In QML you can sent a UiMessage using `id.sendUiMessageString(100,
  "somestring")`. We will convert the given data into a latin1 string
  and construct a `UiMessage` that we forward to the widget. So, lets
  say you created a histogram widget and you initialized it in your
  QML with an id `histogram`, then you can do
  `histogram.sendUiMessageString(100, "something")` and the widget
  itself (e.g. the openFramworks code) will receive the `UiMessage` in
  it's `onUiMessage()` listener function.

  This function was created to communicate between the GUI layer and
  the widget layer. 
 */
void QtOfExternalWidget::sendUiMessageString(unsigned int eventType, const QString& str) {
  UiMessage msg;
  msg.type = eventType;
  msg.s = str.toLatin1().data();
  qtof_widget_send_message(ref, msg);
}

QString QtOfExternalWidget::getJson(unsigned int what) {
  
  std::string json;
  if (0 != qtof_widget_get_json(ref, what, json)) {
    qWarning("Failed to get json from widget.");
    return "";
  }

  QString result(json.c_str());
  return result;
}

/* ---------------------------------------------------- */

QtOfExternalWidgetCleanupRunnable::QtOfExternalWidgetCleanupRunnable(int ref)
  :ref(ref)
{
}

void QtOfExternalWidgetCleanupRunnable::run() {
  qtof_widget_destroy(ref);
}

/* ---------------------------------------------------- */
