#pragma once
#include "main.h"
namespace Hal {
  class Gpio {
  public:
    Gpio(GPIO_TypeDef* gpioGroup, uint16_t gpioPin) : gpioGroup(gpioGroup), gpioPin(gpioPin) {
      
    }
    
    Gpio(Gpio&) = delete;
    
    void operator=(int pinState) {
      HAL_GPIO_WritePin(gpioGroup, gpioPin, (GPIO_PinState)pinState);
    }
    
  private:
    GPIO_TypeDef* gpioGroup;
    uint16_t gpioPin;
  };
}