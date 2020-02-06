#pragma once
#include "GuiLite.h"
#include <string>
#include <sstream>
#include <iomanip>
namespace App {
using namespace std;
class Page1Table : public c_table {
public:
  Page1Table(c_dialog* page1) {
    connect(page1, 233);
  }
  
  Page1Table(Page1Table& page1) = delete;
  
  virtual void on_init_children() override {
    //256 右边留6个像素
    set_col_num(kColNum);
    set_row_num(kRowNum);
    set_col_width(kColWidth);
    set_col_width(0, m_wnd_rect.Width() -  (kColNum - 1) * kColWidth - kPaddingRight);
    set_row_height(kRowHeight);
  }
  
  virtual void connect(c_wnd* parent, uint16_t id) {
    auto rect = c_rect { };
    parent->get_wnd_rect(rect);
    c_table::connect(parent, 233, nullptr, 0, 0, rect.Width(), rect.Height());
  }
  
  virtual void on_paint() override { //这个on_paint函数可以手动调用哒
    set_sheet_align(ALIGN_LEFT);
    set_item(0, 0, ""); //调用函数后会立即作图
    set_item(1, 0, "电池");
    set_item(2, 0, "充电输入");
    set_item(3, 0, "TYPEC输出");
    set_item(4, 0, "TYPEA输出");
    
    set_sheet_align(ALIGN_HCENTER);
    set_item(0, 1, "U");
    set_item(0, 2, "I");
    set_item(0, 3, "容量");
    set_item(0, 4, "温度");
    
    set_sheet_align(ALIGN_RIGHT);
    
    for (auto i = 1; i < 5; i++) {
      for (auto j = 1; j < 5; j++) {
        set_item(i, j, "");
      }
    }
  }
  
  void set_item(int row, int col, string str) {
    c_table::set_item(row, col, (char*)str.c_str(), kRowColor);
    
  }
  
  virtual c_rect get_item_rect(int row, int col) override {
    auto rect = c_table::get_item_rect(row, col);
    rect.m_left += kItemPaddingHorizental;
    rect.m_right -= kItemPaddingHorizental * 2;
    return rect;
  }
  
  
  auto operator [](int idx) {
    return Row { this, idx };
  }
  
  class Row {
    public:
      Row(Page1Table* parent, int row) : parent(parent), row(row) { }
      auto operator [](int idx) {
        
        return Col { this, idx };
      }
      class Col {
      public:
        Col(Row* row, int col) : row(row), col(col) {
          *this << setiosflags(ios::fixed) << setprecision(2);
        }
          
        template<class T>
        Col& operator<<(T other) {
          ss << other;
          return *this;
        }
        
        ~Col() {
          row->parent->set_item(row->row, col, ss.str());
        }
          
      private:
        stringstream ss;
        Row* row;
        int col;
      };
    private:
      Page1Table* parent;
      int row;
    };
  
  static const int kColNum = 5, kRowNum = 5, kColWidth = 45, kRowHeight = 16, kPaddingRight = 6, kItemPaddingHorizental = 2;
  static const uint32_t kRowColor = GL_RGB(57, 56, 66);
};
}