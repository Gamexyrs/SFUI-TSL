#pragma once

#define __GY_PLUGIN_FILER_VERSION__ L"V1.0_DEV"

#include "DirectorySelector.hpp"
#include <algorithm>

#define DocType Filer::Require

namespace gy_plugin {
  typedef class Filer {
  protected: Filer(void) = delete;
            ~Filer(void) = delete;
  
  public:
    enum Require {
      File = 1,
      Dir  = 2,
    };
    struct ActivitySettings {
      std::wstring path{L"/storage/emulated/0/"};
      int require{Require::File};
      
      bool display_hiddenFile{false};
      bool darkMode{false};
      
      sf::Font* font{nullptr};
    };
  
    static func activity_start(const ActivitySettings&) -> sf::String;
  
  protected:
    static func activity_run(const ActivitySettings&) -> sf::String;
    
  } Filer;
    
  //>>>
  
  func Filer::activity_start(const ActivitySettings& usrs) -> sf::String {
   if(usrs.require != 1
   && usrs.require != 2) return L"";
   if(usrs.font == nullptr)
     const_cast<ActivitySettings&>(usrs).font = &sf::Renderable::getDefaultFont();
    
    const auto& ScvR{sf::ScrollableView::getCopyableData()};
    
    sf::String __tmp{};
    try {__tmp = Filer::activity_run(usrs);} catch(...) {}
    
    sf::ScrollableView::setCopyableData(ScvR);
    
    sf::ScrollableView::resetHorizontalView();
    sf::ScrollableView::resetVerticalView();
    
    return __tmp;
  }
  
  func Filer::activity_run(const ActivitySettings& usrs) -> sf::String {
    DirSelector __dir{usrs.path};
    
reload:
    std::vector<std::wstring> __target{-__dir};
    std::sort(__target.begin(), __target.end());
    if(!usrs.display_hiddenFile) {
      __target.erase(std::remove_if(__target.begin(), __target.end(),
        [](const std::wstring& v)-> bool { return v.front()==L'.'; }),
      __target.end());
    }
    for(auto i = __target.begin(); i != __target.end(); ++i) {
      if(!std::isdigit(i->front())) {
        if(i != __target.begin()) {
          std::move(__target.begin(), i, std::back_inserter(__target));
        } break;
      }
    }
    __target.erase(std::remove_if(__target.begin(), __target.end(),
      [](const std::wstring& v) -> bool { return v.empty(); }),
    __target.end());
    
    sf::Text __cpr{std::wstring{L"GyPlug-in_Filer("}
      + __GY_PLUGIN_FILER_VERSION__ + L")"
      + L", insider: GyPlug-in_DirectorySelector("
      + __GY_PLUGIN_DIRECTORY_SELECTOR_VERSION__ + L")", *usrs.font, 20}; {
      __cpr.setFillColor(sf::ColorEx::Grey);
      __cpr.move({50, 0});
    }
    
    sf::Vector2f __path_default_size{_RendererSize.x, 180};
    sf::PushBtn __path{{{}, __path_default_size}}; {
      __path.setText(&__dir, *usrs.font, 40);
      __path.setTextAlign(sf::Align::L) += {30, 40};
      __path.setStateColor(sf::Color::Transparent);
      __path.getText().setLetterSpacing(0.75);
      __path.getBase().setOutlineThickness(5);
      
      for(size_t t{2}; __path.getNextPosition().x >= _Renderer.getSize().x && t <= __dir.length(); ++t)
        __path.setTextString(L"..." + __dir[{t, __dir.length()}]);
    } sf::String __shortyPath{__path.getTextString()};
    
    std::vector<sf::PushBtn> __blocks{__target.size() + 1}; {
      for(size_t i = 0; i < __target.size() + 1; ++i) {
        __blocks[i].setSize({_RendererSize.x, 120});
        __blocks[i].setBuilder(i ? __blocks[i-1] : __path,
          sf::Vector2f{0.0f, i ? 5.0f : 40.0f},
          sf::Vector2b{false, true});
        __blocks[i].setText(i ? __target[i-1] : (__dir.isPreviousValid() ? L"← 上一项" : L"← 退出"), *usrs.font);
        __blocks[i].setTextAlign(sf::Align::L) += {i ? 100.0f : 50.0f, i ? -5.0f : 0.0f};
        __blocks[i].setTag<int>(i ? (__dir.check(std::move(__target[i-1])) ? 2 : 1) : 0);
        if(__blocks[i].getTag<int>() == 1 && !(usrs.require & Require::File))
           __blocks[i].setTextColor(sf::ColorEx::Grey);
        if(__blocks[i].getTag<int>() == 2)
           __blocks[i].setTextString(__blocks[i].getTextString() + L"     #→");
      }
    }
    
    sf::Div __useDir_Mask{{{}, {_RendererSize.x, 160}}, 0}; {
      __useDir_Mask.__ATTRIBUTE__.__VISIBLE__ = usrs.require & Require::Dir;
      __useDir_Mask.getBase().setFillColor(sf::ColorEx::LightGrey);
    }
    sf::PushBtn __useDir{{_RendererSize.x - 40, 100}, __useDir_Mask, 20}; {
      __useDir.setStateColor(sf::Color  ::     Blue, sf::BtnState::Pressed);
      __useDir.setStateColor(sf::ColorEx::LightBlue, sf::BtnState::None);
      __useDir.__ATTRIBUTE__.__VISIBLE__ = usrs.require & Require::Dir;
      __useDir.setText(L"使用此文件夹", *usrs.font, 40);
      __useDir.setTouchRectDeviat({0, 60});
      __useDir.setTouchRectScale ({1, 2});
      __useDir.setTextAlign(sf::Align::C);
    }
    
    // 滑屏
    if(__blocks.back().getRoot().y + 200 > _RendererSize.y) {
      sf::ScrollableView::setVerticalMoveEnable(true);
      sf::ScrollableView::setVerticalMax({0, __blocks.back().getRoot().y + 200});
    } else sf::ScrollableView::setVerticalMoveEnable(false);
    
    // 显示
    sf::Event event{};
    while(sf::Renderable::loopBegin(usrs.darkMode ? sf::ColorEx::Grey : sf::Color::White)) {
      sf::TouchEvent::pollEvent(event);
      while(_Renderer.pollEvent(event)) {
        if(__ACTIV_EXIT__(event)) return L"";
        if(__path.pollEvent_if(event)) {
          sf::ScrollableView::resetVerticalView();
          
          // Click path
          if(__path.getAutoLineBreakEnable()) {
            __path.setTextString(__shortyPath);
            __path.setSize(__path_default_size);
            __path.setAutoLineBreakEnable(false);
            __path.__ATTRIBUTE__.__VISIBLE_BASE__ = true;
            sf::ScrollableView::setVerticalMoveEnable(
              __blocks.back().getRoot().y + 200 > _RendererSize.y);
          } else {
            __path.setSize(_RendererSize
                  - sf::Vector2f{40, 0});
            __path.setTextString(&__dir);
            __path.setAutoLineBreakEnable(true);
            __path.__ATTRIBUTE__.__VISIBLE_BASE__ = false;
            sf::ScrollableView::setVerticalMoveEnable(false);
          }
        }
         
        // Click doc
        if(__useDir.pollEvent_if(event)) return &__dir;
        for(auto& i : __blocks) {
          if(i.getTextColor() != sf::ColorEx::Grey
          && i.pollEvent_if(event)) {
            sf::String __filename{i.getTextString().substring(0, i.getTextString().find(" "))};
            
            if(!i.getTag<int>()) {
              if(__dir.isPreviousValid()) {
                 __dir--; goto reload;
              } else return L"";
            } else if(__dir.check(__filename)) {
                      __dir /= __filename.toWideString() + L"//";
              goto reload;
            } else return &__dir + __filename;
          }
        }
      }
      
      __useDir_Mask.align(sf::Align::B);
      __useDir.     align(sf::Align::C);
      
      sf::Renderable::draw(__path, __cpr);
      for(const auto& i : __blocks)
        _Renderer.draw(i);
      sf::Renderable::draw(__useDir_Mask, __useDir);
        
      _Renderer.display();
      
      sf::ScrollableView::pollEvent();
    } return L"";
  }
}
