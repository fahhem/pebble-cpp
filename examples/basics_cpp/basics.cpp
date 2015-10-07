#include "pebble.h"

class PebbleApp {
 public:
  void window_load(Window *window) {
    CPPWindow root_window(window);
    root_window.set_click_config_provider(
       (ClickConfigProvider)&App<PebbleApp>::click_config_provider);

    CPPLayer window_layer(root_window.get_root_layer());
    auto bounds = window_layer.get_bounds();
    title_layer.reset(new CPPTextLayer(0, 72, bounds.size.w, 20));
    title_layer->set_text("Press a button");
    title_layer->set_text_alignment(GTextAlignmentCenter);
    window_layer.add_child(*title_layer);
  }

  void window_unload(Window *window) {
    title_layer.release();
  }

  void click_config_provider(void *ctx) {
    CPPWindow::single_click_subscribe<
        PebbleApp, &PebbleApp::select_click_handler>(BUTTON_ID_SELECT);
    CPPWindow::single_click_subscribe<
        PebbleApp, &PebbleApp::up_click_handler>(BUTTON_ID_UP);
    CPPWindow::single_click_subscribe<
        PebbleApp, &PebbleApp::down_click_handler>(BUTTON_ID_DOWN);
  }

  void select_click_handler(ClickRecognizerRef rec) {
    title_layer->set_text("Select");
  }

  void up_click_handler(ClickRecognizerRef rec) {
    title_layer->set_text("Up");
  }
  
  void down_click_handler(ClickRecognizerRef rec) {
    title_layer->set_text("Down");
  }
  
 private:
  std::unique_ptr<CPPTextLayer> title_layer;
};

extern "C" int main(void) {
  App<PebbleApp> app;
  app_event_loop();
}
