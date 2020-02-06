#pragma once
#include "GuiLite.h"
#include <string>
#include <initializer_list>
#include <map>
#include <vector>

namespace App {
using namespace std;

class MainWindow : protected c_wnd {
public:
  MainWindow(c_surface* surface) : pages(this) {
    set_surface(surface);
    connect(nullptr, MainWindow::Id::ROOT, nullptr, 0, 0, surface->get_width(), surface->get_height(), nullptr);
  }
  
  
  void show() {
    show_window();
  }
  
  virtual void on_init_children() override {
    
  }
  
  virtual void on_paint() override {
    
  }
  
  void on_button_clicked(int ctrl_id, int param) {
    
  }
  
  class PageManager {
  public:
    PageManager(MainWindow* p) : parent(p) { }
    
    auto add(string title) -> c_dialog* {
      auto size = tabs.size();
      auto tabTitle = new c_button();
      tabTitle->connect(parent, parent->getNextId(), title.c_str(), size * kTabTitleWidth, 0, kTabTitleWidth, kTabTitleHeight, nullptr);
      auto surface = parent->get_surface();
      tabTitle->connect(parent, parent->getNextId(), nullptr, 0, 0, surface->get_width(), surface->get_height() - kTabTitleHeight, nullptr);
      auto tabContent = new c_dialog();
      tabs.insert({ tabTitle, tabContent });
      return tabContent;
      
    }
  private:
    map<c_button*, c_dialog*> tabs;

    MainWindow* parent;
  } pages;
  
private:
  int getNextId() {
    return ++curId;
  }
  
public:
  enum Id { 
    ROOT = 1,
  };
  
  
  
  int curId = ROOT;
  
  GL_DECLARE_MESSAGE_MAP()
};

}

