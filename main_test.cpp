#include <SFML/Graphics.hpp>
#include <SFUI/Graphics.hpp>
#include <FakePay.hpp>
#include <Filer.hpp>
#include <streambuf>
#include <fstream>

func main_test_FakePay(void) -> int {
  sf::ContextSettings settings(0, 0, 1);
  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "", sf::Style::Default, settings);
  window.setFramerateLimit(0);
  
  sf::Font Font; Font.loadFromFile("./res/MiSansVF.ttf");
  sf::Renderable::setRenderer(window);
  sf::Renderable::setDefaultFont(Font);
  
  sf::PushBtn __up{{{}, {260, 100}}, 10}; {
    __up.setStateColor({46, 189, 115, 180}, sf::BtnState::Pressed);
    __up.setStateColor({46, 189, 115}, sf::BtnState::None);
    __up.align(sf::Align::B) -= {0, 120};
    __up.setOutlineThickness(0);
  }
  
  sf::Event event; FakePay __pay{Font, Font, "./res/coin.png", "./res/accept.png"};
  while(sf::Renderable::loopBegin(sf::ColorEx::Grey | 80)) {
    sf::TouchEvent::pollEvent(event);
    while(window.pollEvent(event)) {
      sf::KeyboardEvent::pollEvent(event);
      if(__ACTIV_EXIT__(event)) window.close();
      if(__up.pollEvent_if(event)) __pay.call(L"648.00");
      
      __pay.pollEvent(event);
    }
    
    sf::Renderable::draw(__up);
    
    __pay.draw();
    
    _Renderer.display();
  } return 0;
}

func main_test_Filer(void) -> int {
  sf::ContextSettings settings(0, 0, 1);
  sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[1], "", sf::Style::Fullscreen, settings);
  window.setFramerateLimit(60);
  
  sf::ScrollableView::setVerticalMoveEnable(true);
  sf::ScrollableView::setVerticalMax({0, 10000});
  
  sf::Font Font
  if(!Font.loadFromFile("./res/MiSansVF.ttf")) return 1;
  sf::Renderable::setRenderer(window);
  sf::Renderable::setDefaultFont(Font);
  
  sf::PushBtn Div({{}, {0, 200}}, 10); {
    Div.setPerWidth("40%");
    Div.setText(L"选择文件", Font, 100);
    Div.setTextAlign(sf::Align::T);
    Div.setTextDeviat({-10, -5});
    Div.align(sf::Align::C) -= {0, 700};
    Div.setAutoLineBreakEnable(true);
  }
  
  sf::TextDiv Txt({{0, 100}, _RendererSize}); {
    Txt.setText(L"null", Font, 20);
    Txt.setAutoLineBreakEnable(true);
    Txt.__ATTRIBUTE__.__VISIBLE_BASE__ = false;
  }
  
  sf::Event event;
  while(sf::Renderable::loopBegin()) {
    sf::TouchEvent::pollEvent(event);
    while(window.pollEvent(event)) {
      if(__ACTIV_EXIT__(event)) {
        window.close();
      }
      
      if(Div.pollEvent_if(event)) {
        std::wifstream ifs{};
        ifs.open(gy_plugin::Filer::activity_start(
                 gy_plugin::Filer::ActivitySettings{
          .require = gy_plugin::DocType::Dir
                   ^ gy_plugin::DocType::File,
          .display_hiddenFile = true,
        }).toAnsiString());
        std::wstring str{(std::istreambuf_iterator<wchar_t>(ifs)),
                          std::istreambuf_iterator<wchar_t>()};
        if(!str.empty()) {Txt.setTextString(str);} ifs.close();
      }
      
    }
    sf::Renderable::draw({&Div, &Txt});
    
    _Renderer.display();
    
    sf::ScrollableView::pollEvent();
  } return 0;
}
*/
