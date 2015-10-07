  #include "pebble.h"

class PebbleApp {
 public:
  void window_load(Window *window) {
    CPPWindow root_window(window);

    CPPLayer window_layer(root_window.get_root_layer());
    auto bounds = window_layer.get_bounds();

    bitmap_layer.reset(new CPPBitmapLayer(0, 0, bounds.size.w, bounds.size.h,
                                          RESOURCE_ID_IMAGE_NO_LITTER));
    bitmap_layer->set_bitmap(bitmap_layer->image_bitmap_);
    bitmap_layer->set_alignment(GAlignCenter);
    window_layer.add_child(*bitmap_layer);
  }

  void window_unload(Window *window) {
    bitmap_layer.release();
  }
  
 private:
  std::unique_ptr<CPPBitmapLayer> bitmap_layer;
  std::unique_ptr<GBitmap> bitmap_image;
};

extern "C" int main(void) {
  App<PebbleApp> app;
  app_event_loop();
}