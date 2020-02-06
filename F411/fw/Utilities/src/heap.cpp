#include "main.h"
#include "stdlib.h"
void * operator new(size_t size) {
  __disable_irq();
  return malloc(size);
  __enable_irq();
}

void operator delete(void* p) noexcept {
  __disable_irq();
  free(p);
   __enable_irq();
}

void operator delete[](void * p) noexcept {
  __disable_irq();
  free(p);
  __enable_irq();
}