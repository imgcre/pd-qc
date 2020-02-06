#include "sh1122.hpp"
#include "main.h"
#include "main_window.hpp"
#include <utility>
#include "page1_table.hpp"
#include <sstream>
#include <iomanip>

#define GUILITE_ON
#include "GuiLite.h"

using namespace Bsp;
using namespace App;
using namespace std;

extern const FONT_INFO Arial_16B;

namespace App {
void loadResource() {
  c_theme::add_font(FONT_DEFAULT, &Arial_16B);
  
  c_theme::add_color(COLOR_WND_FONT, GL_RGB(255, 255, 255));
	c_theme::add_color(COLOR_WND_NORMAL, GL_RGB(59, 75, 94));
	c_theme::add_color(COLOR_WND_PUSHED, GL_RGB(33, 42, 53));
	c_theme::add_color(COLOR_WND_FOCUS, GL_RGB(43, 118, 219));
	c_theme::add_color(COLOR_WND_BORDER, GL_RGB(46, 59, 73));
}
}



extern "C" {
void startUiTask(void *argument) {
  static auto& sh1122 = Sh1122::instance (
    &hspi1, 
    new Gpio { SH1122_CS_GPIO_Port, SH1122_CS_Pin },
    new Gpio { SH1122_DC_GPIO_Port, SH1122_DC_Pin },
    new Gpio { SH1122_NRST_GPIO_Port, SH1122_NRST_Pin }
  );
  
  sh1122.init();
  
  loadResource();
  
  static auto display = c_display { 
    nullptr,
    sh1122.kWidth,
    sh1122.kHeight,
    sh1122.kWidth,
    sh1122.kHeight,
    2,
    1, //surface count
    new EXTERNAL_GFX_OP {
      .draw_pixel = [](int x, int y, unsigned int rgb) {
        auto max = std::max({ rgb & 0xff, (rgb >> 8) & 0xff, (rgb >> 16) & 0xff });
        sh1122.drawPixel(x, y, (max >> 4) & 0x0f);
      },
      .fill_rect = [](int x0, int y0, int x1, int y1, unsigned int rgb) {
        auto max = std::max({ rgb & 0xff, (rgb >> 8) & 0xff, (rgb >> 16) & 0xff });
        sh1122.fill(x0, y0, x1, y1, (max >> 4) & 0x0f);
      },
    },
  };
  
  auto surface = display.alloc_surface(Z_ORDER_LEVEL_3); //最大z序
  surface->set_active(true);
  
  static MainWindow window(surface);
  
  //window.addPages({"指示内容", "指示状态", "显示状态", "显示"});
  
  
  auto page1 = window.pages.add("指示内容");
  auto& page1Table = *new Page1Table(page1);

  page1Table[1][1] << 10.2 << "V";
  page1Table[2][1] << 5.28 << "V";
  page1Table[3][1] << 4.9 << "V";
  page1Table[4][1] << 5.11 << "V";
  
  
  window.show();
  
  
  
  while (1) {
    thread_sleep(10);
  }
  
  
  //load_resource
  //static auto display = c_display { 
  //  nullptr, 
  //};
}
}
