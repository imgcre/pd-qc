#pragma once
#include "GuiLite.h"
#include <string>
#include <list>
#include <vector>
#include <main_window.hpp>



namespace App {
using namespace std;
class TabBtn : public c_wnd {
protected:
  virtual void on_paint() override {
    //TODO: 在这里准备重绘
    
  }
  
  //void tick
  //只管理被选中和取消选中状态
  class Title {
  public:
    Title(const string str, c_surface* surface, int y) : 
      str(str), 
      state(State::Selected),
      curVisibleWidth(kMinWidth),
      kSurface(surface),
      y(y) {
        int textWidth, textHeight;
        c_word::get_str_size(str.c_str(), c_theme::get_font(FONT_DEFAULT), textWidth, textHeight);
        kMaxWidth = textWidth + kPaddingHorizontal * 2;
        kHeight = textHeight + kPaddingVertical * 2;
    }

    void select() {
      state = State::Selecting;
    }
    
    void deselect() {
      state = State::Deselecting;
    }
    
    void tick() {
      switch (state) {
        case State::Selecting:
          curVisibleWidth += kStep;
          if (curVisibleWidth >= kMaxWidth) {
            curVisibleWidth = kMaxWidth;
            state = State::Selected;
          }
          break;
        case State::Deselecting:
          curVisibleWidth -= kStep;
          if(curVisibleWidth <= kMinWidth) {
            curVisibleWidth = kMinWidth;
            state = State::Deselected;
          }
          break;
        default:
        return;
        break;
      }
      auto surfaceWidth = kSurface->get_width();
      auto rect = c_rect { surfaceWidth - curVisibleWidth,  y, surfaceWidth - 1, y + kHeight};
      c_word::draw_string_in_rect(
        kSurface, Z_ORDER_LEVEL_3, str.c_str(), rect, c_theme::get_font(FONT_DEFAULT), 
        c_theme::get_color(COLOR_WND_FONT), c_theme::get_color(COLOR_WND_NORMAL)
      );
    }
    
    int kMaxWidth;
    int kHeight;
    c_surface* kSurface;
    
  private:
    enum class State { 
      Deselected,
      Selecting,
      Selected,
      Deselecting,
    };
    
    State state;
    int curVisibleWidth;
    
    int y;

    const string str;
    
    
    static const int 
      kPaddingHorizontal = 2,
      kPaddingVertical = 0;
  };
  
  void addTitle(const string str) {
    auto title = new Title(str, get_surface(), kPaddingTop + titles.size() * c_theme::get_font(FONT_DEFAULT)->height);
    titles.push_back(title);
  }
  
  void tick() {
    //计算矩形区大小
    if (titles.size() == 0)
      return;
    
    auto maxWidth = 0;
    for (auto title : titles) {
      if (title->kMaxWidth > maxWidth) {
        maxWidth = title->kMaxWidth;
      }
    }
    auto height = titles.size() * titles.front()->kHeight;
    auto surface = get_surface();
    
    auto rect = c_rect(surface->get_width() - maxWidth, kPaddingTop, 
      surface->get_width() - 1, kPaddingTop + height);
    
    
    get_surface()->set_frame_layer_visible_rect(rect, Z_ORDER_LEVEL_3);
    
    //TODO: 复制底下图层的内容
    
    for (auto title : titles) {
      title->tick();
    }
    
  }
  
private:
  list<Title*> titles;
  
  static const int 
    kZOrder = Z_ORDER_LEVEL_MAX - 1,
    kPaddingTop = 10,
    kMinWidth = 2,
    kStep = 3;
};
}