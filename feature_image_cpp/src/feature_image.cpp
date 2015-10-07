#include "pebble.h"

class PebbleApp {
 public:
  void window_load(Window *window) {
    CPPWindow root_window(window);

    CPPLayer window_layer(root_window.get_root_layer());
    //auto bounds = window_layer.get_bounds();

    //s_image_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NO_LITTER);

  }

  void window_unload(Window *window) {
    bitmap_layer.release();
  }
  
 private:
  std::unique_ptr<CPPBitmapLayer> bitmap_layer;
};

extern "C" int main(void) {
  App<PebbleApp> app;
  app_event_loop();
}