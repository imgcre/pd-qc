#pragma once
#include "main.h"
#include "singleton.hpp"
#include "spi_device.hpp"

namespace Bsp {
  using namespace Utilities;
  using namespace Hal;
  using namespace std;
  class Sh1122 : public Singleton<Sh1122> {
    friend class Singleton<Sh1122>;
    Sh1122(SPI_HandleTypeDef* hSpi, Gpio* csPin, Gpio* dcPin, Gpio* nrstPin) : spi(hSpi, csPin), dcPin(*dcPin), nrstPin(*nrstPin) { }
  public:
    void init() {
      nrstPin = 0;
      osDelay(100);
      nrstPin = 1;
      
      write({0xAE, 0xB0, 0x00, 0x10, 0x00, 0xD5, 0x50, 0xD9, 0x22, 0x40, 
          0x81, 0xFF, 0xA0, 0xC0, 0xA4, 0xA6, 0xA8, 0x3F, 0xAD, 0x80, 
          0xD3, 0x00, 0xDB, 0x30, 0xDC, 0x30, 0x33});
      fill(0, 0, kWidth, kHeight, 0);
      write(0xAF);
      
    }
    
    void fill(int x0, int y0, int x1, int y1, uint8_t color) {
      for (int y = y0; y < y1; y++) {
        setPos(x0, y);
        for (int x = x0; x < x1; x++) {
          write(color, WriteType::Data);
        }
      }
    }
    
    void drawPixel(int x, int y, uint8_t color) {
      setPos(x, y);
      write(color, WriteType::Data);
    }
    
    void setPos(uint8_t x, uint8_t y) {
      write({0xB0, y, uint8_t(((x & 0xf0) >> 4) | 0x10), uint8_t(x & 0x0f)});
    }
    
    const int kWidth = 256;
    const int kHeight = 64;
    
  private:
    enum class WriteType;
    
    void write(uint8_t data, WriteType type = WriteType::Command) {
      dcPin = type == WriteType::Command;
      spi = data;
      dcPin = 1;
    }
    
    void write(initializer_list<uint8_t> list, WriteType type = WriteType::Command) {
      for (auto data : list) {
        write(data, type);
      }
    }
    
    enum class WriteType { 
      Command,
      Data
    };
    
    SpiDevice spi;
    Gpio &dcPin, &nrstPin;
  };
}
