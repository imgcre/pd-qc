#include "program.hpp"
#include "main.h"
#include "sc8812a.hpp"
#include "stdio.h"
#include "gpio.hpp"
#include "spi_device.hpp"
using namespace Bsp;
using namespace Hal;

template<class VBatProperty>
class Screen {
public:
  
  Screen(VBatProperty& vbat) : vbat(vbat) {
    
  }
  
  void draw() {
    float vBat = vbat; //实际求值
    
    printf("vbat val: %f", vBat);
  }
  
  VBatProperty& vbat;
  
};

extern "C" {
void StartDefaultTask(void *argument) {
  auto& sc8812a = Sc8812a::instance(&hi2c1);
  
  float vBus = sc8812a.vBus;
  float vBat = sc8812a.vBat;
  float iBus = sc8812a.iBus;
  float iBat = sc8812a.iBat;
  
  auto s = Screen(sc8812a.vBat);
  s.draw();
  
  Gpio t = { SH1122_CS_GPIO_Port, SH1122_CS_Pin };
  auto f = SpiDevice(&hspi1, &t);
  
}
}

