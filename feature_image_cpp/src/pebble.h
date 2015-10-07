#ifndef PEBBLE_CPP_H
#define PEBBLE_CPP_H

extern "C" {
#include <pebble.h>
}

#include <functional>
#include <memory>
#include <utility>

#define PROXY_METHOD(ReturnType, MethodName, Prefix, WrappedObject) \
  template<class...Args> \
  ReturnType MethodName(Args&&... args) { \
    return Prefix ## MethodName(WrappedObject, std::forward<Args>(args)...); \
  }

template<typename PebbleAppT> class App;

class CPPWindow {
 public:
  CPPWindow();
  explicit CPPWindow(Window *window);

  ~CPPWindow();

  #define WINDOW_PROXY_METHOD(R, M) PROXY_METHOD(R, M, window_, window_)

  void set_window_handlers(
      WindowHandler load,
      WindowHandler unload = nullptr,
      WindowHandler appear = nullptr,
      WindowHandler disappear = nullptr);

  template<typename PebbleAppT, void (PebbleAppT::*Method)(ClickRecognizerRef)>
  static void single_click_subscribe(ButtonId button) {
    auto handler = &App<PebbleAppT>::template click_handler<Method>;
    window_single_click_subscribe(button, static_cast<ClickHandler>(handler));
  }

  // push(animated);
  WINDOW_PROXY_METHOD(void, stack_push);

  // set_click_config_provider(ClickConfigProvider provider);
  // set_click_config_provider_with_context(ClickConfigProvider provider, void* ctx);
  WINDOW_PROXY_METHOD(void, set_click_config_provider);
  WINDOW_PROXY_METHOD(void, set_click_config_provider_with_context);

  // Layer* get_root_layer();
  WINDOW_PROXY_METHOD(Layer*, get_root_layer);

  // bool get_fullscreen();
  WINDOW_PROXY_METHOD(bool, get_fullscreen);

  template<typename UserData>
  UserData* get_user_data() {
    return (UserData*)window_get_user_data(window_);
  }

  // bool is_loaded();
  WINDOW_PROXY_METHOD(bool, is_loaded);

  // void set_background_color(GColor color);
  WINDOW_PROXY_METHOD(void, set_background_color);

  // void set_fullscreen(bool enabled);
  WINDOW_PROXY_METHOD(void, set_fullscreen);

  // void set_status_bar_icon(const GBitmap* icon);
  WINDOW_PROXY_METHOD(void, set_status_bar_icon);

 private:
  Window *window_;
  bool window_owned_;
};

class CPPLayer {
 public:
  CPPLayer();
  explicit CPPLayer(Layer *layer);
  ~CPPLayer();

  #define LAYER_PROXY_METHOD(R, M) PROXY_METHOD(R, M, layer_, layer_)

  void add_child(CPPLayer &layer);

  // GRect get_bounds();
  LAYER_PROXY_METHOD(GRect, get_bounds);

  // void set_bounds(GRect);
  LAYER_PROXY_METHOD(void, set_bounds);

  // bool get_clips();
  LAYER_PROXY_METHOD(bool, get_clips);

  // void get_clips(bool);
  LAYER_PROXY_METHOD(void, set_clips);

  // GRect get_frame();
  LAYER_PROXY_METHOD(GRect, get_frame);

  // void set_frame(GRect);
  LAYER_PROXY_METHOD(void, set_frame);

  // bool get_hidden();
  LAYER_PROXY_METHOD(bool, get_hidden);

  // void set_hidden(bool);
  LAYER_PROXY_METHOD(void, set_hidden);

  // Window* get_window();
  LAYER_PROXY_METHOD(Window*, get_window);

  // void mark_dirty();
  LAYER_PROXY_METHOD(void, mark_dirty);

 protected:
  Layer *layer_;
  bool layer_owned_;
};

class CPPTextLayer : public CPPLayer {
 public:
  explicit CPPTextLayer(int16_t x, int16_t y, int16_t w, int16_t h);

  #define TEXT_LAYER_PROXY_METHOD(R, M) PROXY_METHOD(R, M, text_layer_, text_layer_)

  // void set_text_alignment(GTextAlignment);
  TEXT_LAYER_PROXY_METHOD(void, set_text_alignment);

  // GSize get_content_size();
  TEXT_LAYER_PROXY_METHOD(GSize, get_content_size);

  // void set_size(GSize max_size);
  TEXT_LAYER_PROXY_METHOD(void, set_size);

  // const char* get_text();
  TEXT_LAYER_PROXY_METHOD(const char*, get_text);

  // void set_text(char*);
  TEXT_LAYER_PROXY_METHOD(void, set_text);

  // void set_background_color(GColor);
  TEXT_LAYER_PROXY_METHOD(void, set_background_color);

  // void set_text_color(GColor);
  TEXT_LAYER_PROXY_METHOD(void, set_text_color);

  // void set_font(GFont);
  TEXT_LAYER_PROXY_METHOD(void, set_font);

  // void set_overflow_mode(GTextOverflowMode);
  TEXT_LAYER_PROXY_METHOD(void, set_overflow_mode);

 private:
  TextLayer *text_layer_;
};

class CPPBitmapLayer : public CPPLayer {
  public:
    explicit CPPBitmapLayer(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t resource_id);
    
    #define BITMAP_LAYER_PROXY_METHOD(R, M) PROXY_METHOD(R, M, bitmap_layer_, bitmap_layer_)

    // const GBitmap* get_bitmap()
    BITMAP_LAYER_PROXY_METHOD(const GBitmap*, get_bitmap);

    // void set_bitmap(const GBitmap*)
    BITMAP_LAYER_PROXY_METHOD(void, set_bitmap)

    // void set_alignment(GAlign)
    BITMAP_LAYER_PROXY_METHOD(void, set_alignment)

    // void set_background_color(GColor)
    BITMAP_LAYER_PROXY_METHOD(void, set_background_color)

    //void set_compositing_mode(GCompOp)
    BITMAP_LAYER_PROXY_METHOD(void, set_compositing_mode)

  private:
    BitmapLayer *bitmap_layer_;
    GBitmapLayer *image_bitmap_;
};

template<typename PebbleAppT>
class App {
 public:
  explicit App(bool animated = true) {
    root_window_.set_window_handlers(
        &App<PebbleAppT>::window_load, &App<PebbleAppT>::window_unload);
    root_window_.stack_push(animated);
  }

  static void window_load(Window *window) {
    app_.window_load(window);
  }
  
  static void window_unload(Window *window) {
    app_.window_unload(window);
  }

  static void click_config_provider(void* ctx) {
    app_.click_config_provider(ctx);
  }

  template<void (PebbleAppT::*Method)(ClickRecognizerRef)>
  static void click_handler(ClickRecognizerRef rec, void* context) {
    (app_.*Method)(rec);
  }

 private:
  CPPWindow root_window_;
  static PebbleAppT app_;
};

template<typename PebbleAppT>
PebbleAppT App<PebbleAppT>::app_;

#endif // PEBBLE_CPP_H
