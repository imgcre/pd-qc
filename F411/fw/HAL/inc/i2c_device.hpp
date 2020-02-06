#pragma once
#include "main.h"

namespace Hal {
  template<uint16_t DevAddress, class RegEnum = void, uint16_t MemAddrSize = I2C_MEMADD_SIZE_8BIT, uint32_t Timeout = HAL_MAX_DELAY>
  class I2cDevice {
    class Element;
  public:
    I2cDevice(I2C_HandleTypeDef* hi2c) : hi2c(hi2c) { }
    
    Element operator[](int addr) {
      return { this, addr };
    }
    
    Element operator[](RegEnum addr) {
      return { this, (int)addr };
    }
    
  private:
    class Element {
    public:
      Element(I2cDevice* i2c, int addr) : i2c(i2c), addr(addr) { }
      
      void operator=(uint8_t data) { 
         HAL_I2C_Mem_Write(i2c->hi2c, DevAddress, addr, MemAddrSize, &data, 1, Timeout);
      }
      
      operator uint8_t() {
        uint8_t data;
        HAL_I2C_Mem_Read(i2c->hi2c, DevAddress, addr, MemAddrSize, &data, 1, Timeout);
        return data;
      }
      
      uint8_t operator*() {
        return *this;
      }
      
    private:
      I2cDevice* i2c;
      int addr;
    };
    I2C_HandleTypeDef* hi2c;
  };
}