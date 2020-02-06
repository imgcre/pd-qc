#pragma once
#include "main.h"
#include "cmsis_os.h"
#include <map>
#include <functional>
#include "callback.hpp"

extern "C" {
  void startExtiCallbackTask(void* argument);
}
namespace Hal {
  using namespace std;
  using namespace Utilities;
  class Exti {
    friend void ::HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
    friend void ::startExtiCallbackTask(void* argument);
  public:
    
    static void reg(uint16_t gpioPin, function<void(void)> callback) {
      __disable_irq();
      registry.insert_or_assign(gpioPin, callback);
      __enable_irq();
    }
  private:
    static map<uint16_t, function<void(void)>> registry;
    static Callback<void()>* callback;
  };
}