#include "pebble.h"

CPPWindow::CPPWindow() : window_owned_(true) {
  window_ = window_create();
};

CPPWindow::CPPWindow(Window *window)
  : window_(window), window_owned_(false) {};

CPPWindow::~CPPWindow() {
  if (window_owned_)
    window_destroy(window_);
};

void CPPWindow::set_window_handlers(
    WindowHandler load,
    WindowHandler unload /* = nullptr */,
    WindowHandler appear /* = nullptr */,
    WindowHandler disappear /* = nullptr */) {
  window_set_window_handlers(window_, (WindowHandlers) {
    .load = load,
    .appear = appear,
    .disappear = disappear,
    .unload = unload
  });
}


CPPLayer::CPPLayer() : layer_owned_(false) {}
CPPLayer::CPPLayer(Layer *layer) : layer_(layer), layer_owned_(false) {}
CPPLayer::~CPPLayer() {
  if (layer_owned_)
    layer_destroy(layer_);
}

void CPPLayer::add_child(CPPLayer &layer) {
  layer_add_child(layer_, layer.layer_);
}

CPPTextLayer::CPPTextLayer(int16_t x, int16_t y, int16_t w, int16_t h)
  : CPPLayer() {
  text_layer_ = text_layer_create((GRect) { .origin = { x, y }, .size = { w, h } });
  layer_ = text_layer_get_layer(text_layer_);
  layer_owned_ = true;
}

