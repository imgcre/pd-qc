#pragma once
#include "main.h"
#include "gpio.hpp"

namespace Hal {
  class SpiDevice {
  public:
    SpiDevice(SPI_HandleTypeDef* hSpi, Gpio* csPin) : hSpi(hSpi), csPin(*csPin) { }
    
    void operator=(uint8_t data) {
      csPin = 0;
      HAL_SPI_Transmit(hSpi, &data, 1, HAL_MAX_DELAY);
      csPin = 1;
    }
    
  private:
    SPI_HandleTypeDef* hSpi;
    Gpio& csPin;
  };
}
