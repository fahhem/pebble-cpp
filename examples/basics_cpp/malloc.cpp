extern "C" {
#include <pebble.h>
}

#include <new>

void* operator new(size_t size) {
  return malloc(size);
}

void operator delete(void *ptr) {
  free(ptr);
}
