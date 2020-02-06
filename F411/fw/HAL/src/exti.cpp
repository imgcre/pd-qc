#include "exti.hpp"


namespace Hal {
  map<uint16_t, function<void(void)>> Exti::registry {};
  Callback<void()>* Exti::callback = nullptr;
}

using namespace Hal;
extern "C" {
  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (Exti::callback == nullptr)
      return;
    
    Exti::callback->invoke([=] {
      auto search = Exti::registry.find(GPIO_Pin);
      if (search != Exti::registry.end()) {
        search->second();
      }
    });
  }

  void startExtiCallbackTask(void* argument) {
    Exti::callback = new Callback<void()>(extiQueueHandle);
    Exti::callback->poll();
  }
 }