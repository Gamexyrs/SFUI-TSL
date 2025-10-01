#define __GY_PLUGIN_FAKEPAY_VERSION__ L"V1.0_DEV"

typedef class FakePayBubble {
  public:
    FakePayBubble(sf::Font& __cn, sf::Font& __en, const std::string& __load_coin_path, const std::string& __load_confirm_path);
  
  _data_public:
    sf::TextDiv __box{{}, 40};
    sf::TextDiv __box_bk{{}, 40};
    
    sf::PushBtn __exit{{{}, {60, 60}}, 10};
    sf::PushBtn __confirm{{{}, {260, 100}}, 10};
    
    sf::TextDiv __num_type{};
    sf::TextDiv __num{};
    
    sf::TextDiv __sepr{{}, 2};
    
    sf::TextDiv __method{{{}, {0, 60}}};
    sf::PushBtn __method_type{{}, 16};
    sf::Texture __coin;
    sf::Texture __co_t;
    sf::Sprite __co;
    
    sf::RenderVector<sf::TextDiv> __pw_box;
    sf::RenderVector<sf::Div>     __pw_cir;
    std::vector<sf::PushBtn> __pw_key{12};
    
    size_t __pwn{0};
  
  _func_public:
    func draw(void) -> void;
    func isShowing(void) -> bool;
    func call(const sf::String&) -> void;
    func pollEvent(const sf::Event&) -> bool;
  
} FakePay;

FakePay::FakePayBubble(sf::Font& __cn, sf::Font& __en, const std::string& __load_coin_path, const std::string& __load_confirm_path) {
  this->__box.setSize({_RendererSize.x * 0.8f, _RendererSize.y * 0.9f});
  this->__box.setTextAlign(sf::Align::T) += {0, 80};
  this->__box.setText(L"DuckCross", __cn, 40);
  this->__box.setFillColor(sf::Color{64, 64, 64});
  this->__box.setTextColor(sf::Color::White);
  this->__box.setRounded(false, sf::Align::B);
  this->__box.setOutlineThickness(0);
  this->__box.align(sf::Align::B) += {0, __box.getSize().y};
  this->__box.mov.setAuto(true);
  this->__box.setTag<int>(0);
  
  this->__box_bk.setSize(this->__box.getSize());
  this->__box_bk.setTextAlign(sf::Align::T) += {0, 40};
  this->__box_bk.setText(L"支付成功", __cn, 40);
  this->__box_bk.setFillColor(sf::Color{64, 64, 64});
  this->__box_bk.setTextColor(sf::Color::Green);
  this->__box_bk.setRounded(false, sf::Align::B);
  this->__box_bk.setOutlineThickness(0);
  this->__box_bk.align(sf::Align::B) += {0, __box_bk.getSize().y};
  this->__box_bk.mov.setAuto(true);
  this->__box_bk.setTag<int>(0);
  
  this->__exit.setBuilder(__box);
  this->__exit.align(sf::Align::LT) += {40, 40};
  this->__exit.setTextAlign(sf::Align::C) -= {6, 24};
  this->__exit.setOutlineThickness(0);
  this->__exit.setText(L"×", __cn, 80);
  this->__exit.setTextColor(sf::Color::White);
  this->__exit.setStateColor(sf::ColorEx::Grey | 80, sf::BtnState::Pressed);
  this->__exit.setStateColor(sf::Color::Transparent, sf::BtnState::None);
  this->__exit.__ATTRIBUTE__.__ALWAYS_TOUCHABLE__ = true;
  
  this->__confirm.setBuilder(__box_bk);
  this->__confirm.align(sf::Align::B) -= {0, 120};
  this->__confirm.setTextAlign(sf::Align::C);
  this->__confirm.setOutlineThickness(0);
  this->__confirm.setText(L"确定", __cn, 60);
  this->__confirm.setTextColor(sf::Color::Black);
  this->__confirm.setStateColor({46, 189, 115, 180}, sf::BtnState::Pressed);
  this->__confirm.setStateColor({46, 189, 115}, sf::BtnState::None);
  this->__confirm.__ATTRIBUTE__.__ALWAYS_TOUCHABLE__ = true;
  
  this->__num.setBuilder(__box);
  this->__num.setTextAlign(sf::Align::C);
  this->__num.setText(L"-1", __en, 100);
  this->__num.getText().setStyle(sf::Text::Style::Bold);
  this->__num.setTextColor(sf::Color::White);
  this->__num.resizeByText();
  this->__num.__ATTRIBUTE__.__VISIBLE_BASE__ = false;
  
  this->__num_type.setBuilder(__num);
  this->__num_type.setTextAlign(sf::Align::C);
  this->__num_type.setText(L"￥", __cn, 60);
  this->__num_type.getText().setStyle(sf::Text::Style::Bold);
  this->__num_type.setTextColor(sf::Color::White);
  this->__num_type.resizeByText();
  this->__num_type.__ATTRIBUTE__.__VISIBLE_BASE__ = false;
  
  constexpr float __sep{40};
  this->__sepr.setSize({__box.getSize().x - __sep * 2, 4});
  this->__sepr.setBuilder(__box);
  this->__sepr.setOutlineThickness(0);
  this->__sepr.setFillColor(sf::ColorEx::Grey | 80);
  this->__sepr.align(sf::Align::T) += {0, 280};
  
  this->__method.setBuilder(__sepr, sf::Vector2f{0, 20});
  this->__method.setText(L"付款方式", __cn, 30);
  this->__method.setTextColor(sf::ColorEx::Grey);
  this->__method.__ATTRIBUTE__.__VISIBLE_BASE__ = false;
  
  this->__coin.loadFromFile(__load_coin_path);
  this->__method_type.setSize({__sepr.getSize().x, 120});
  this->__method_type.setBuilder(__method, sf::Vector2f{0, 52});
  this->__method_type.setTextAlign(sf::Align::L) += {110, 0};
  this->__method_type.setText(L"零钱", __cn, 40);
  this->__method_type.setStateColor({255, 245, 0, 120}, sf::BtnState::Pressed);
  this->__method_type.setStateColor({255, 245, 0, 140}, sf::BtnState::None);
  this->__method_type.setOutlineColor({137, 111, 70, 140});
  this->__method_type.setTexture(&this->__coin);
  this->__method_type.setTexScale(0.7f);
  this->__method_type.setTexAlign(sf::Align::L) += {20, 0};
  this->__method_type.__ATTRIBUTE__.__ALWAYS_TOUCHABLE__ = true;
  
  this->__co_t.loadFromFile(__load_confirm_path);
  this->__co.setTexture(__co_t);
  this->__co.setScale({0.16f, 0.17f});
  
  size_t t{0};
  constexpr float __pwl{90};
  const     float __pws{(__box.getSize().x - __pwl * 6 - __sep * 2 - 1000) / 5.0f};
  constexpr float __pwsc{__pwl * 3.0f / 8.0f};
  const     float __pwkh{(__box.getSize().y - 620) / 4.0f};
  
  this->__pw_box.resize(6);
  for(auto& i : this->__pw_box) {
    i.setBuilder(this->__method_type, sf::Vector2f{500 + (__pws + __pwl) * t, 30}, sf::Vector2b{false, true});
    i.setFillColor(sf::ColorEx::Grey | 100);
    i.setSize({__pwl, __pwl});
    i.setRadius(16);
    ++t;
  } t = 0;
  
  this->__pw_cir.resize(6);
  for(auto& i : this->__pw_cir) {
    i.setBuilder(this->__method_type, sf::Vector2f{500 + (__pws + __pwl) * t + __pwsc, 30 + __pwsc}, sf::Vector2b{false, true});
    i.setSize({__pwl / 4.0f, __pwl / 4.0f});
    i.__ATTRIBUTE__.__VISIBLE__ = false;
    i.setFillColor(sf::Color::White);
    i.setRadius(-1);
    ++t;
  } t = 0;
  
  for(auto& i : this->__pw_key) {
    i.setBuilder(this->__box, sf::Vector2f{(t % 3) * this->__box.getSize().x / 3.0f, (static_cast<int>(t / 3) - 4) * __pwkh}, sf::Vector2b{false, true});
    i.setSize({this->__box.getSize().x / 3.0f, __pwkh});
    i.setStateColor(sf::ColorEx::Grey | 60, sf::BtnState::Pressed);
    i.setStateColor(sf::Color::Transparent, sf::BtnState::None);
    i.setOutlineColor(sf::ColorEx::Grey | 180);
    i.__ATTRIBUTE__.__ALWAYS_TOUCHABLE__ = true;

    i.setTextAlign(sf::Align::C) -= {0, 12};
    i.setTextColor(sf::Color::White);
    i.setTag<int>(t);
    
    if(t < 9) {
      i.setText(std::to_wstring(t + 1), __en, 60);
    } else if(t == 9) {
      i.setStateColor(sf::ColorEx::Grey | 100, sf::BtnState::None);
    } else if(t == 10) {
      i.setText(L"0", __en, 60);
    } else {
      i.setStateColor(sf::ColorEx::Grey | 100, sf::BtnState::None);
      i.setText(L"<", __en, 60);
    } ++t;
  }
}

inline func FakePay::draw(void) -> void {
  if(!this->isShowing()) return;
  
  if(!this->__box.mov.isEnd() && this->__box.getTag<int>() == 1 && this->__box.getRoot().y <= _RendererSize.y) {
      this->__box.align(sf::Align::B);
      this->__box.mov.clear();
  }
  if(!this->__box_bk.mov.isEnd() && this->__box_bk.getTag<int>() == 1 && this->__box_bk.getRoot().y <= _RendererSize.y) {
      this->__box_bk.align(sf::Align::B);
      this->__box_bk.mov.clear();
  }
  if(this->__box.getTag<int>() == -1 && this->__box.getPosition().y >= _RendererSize.y) {
     this->__box_bk.align(sf::Align::B) += {0, this->__box_bk.getSize().y};
     this->__box.   align(sf::Align::B) += {0, this->__box.getSize().y};
     this->__box_bk.setTag<int>(0);
     this->__box.   setTag<int>(0);
     this->__pwn = 0;
  }
  if(this->__box_bk.getTag<int>() == 2 && this->__box_bk.getPosition().y >= _RendererSize.y) {
     this->__box_bk.align(sf::Align::B) += {0, this->__box_bk.getSize().y};
     this->__box.   align(sf::Align::B) += {0, this->__box.getSize().y};
     this->__box_bk.setTag<int>(0);
     this->__box.   setTag<int>(0);
     this->__box.__ATTRIBUTE__.__VISIBLE_TEXT__ = true;
     this->__exit.__ATTRIBUTE__.__VISIBLE__ = true;
     this->__num.setBuilder(this->__box);
     this->__pwn = 0;
  }

  this->__co.setPosition(sf::align(sf::Fm::getSize(this->__co), this->__method_type.getRect(), sf::Align::R) - sf::Vector2f{40, 0});
  for(size_t i{0}; i < 6; ++i) this->__pw_cir[i].__ATTRIBUTE__.__VISIBLE__ = i < this->__pwn;

  if(this->__box_bk.getTag<int>())
    sf::Renderable::draw(this->__box_bk, this->__confirm);
  sf::Renderable::draw(this->__box, this->__exit);
  sf::Renderable::draw(this->__num, this->__num_type, this->__sepr);
  sf::Renderable::draw(this->__method, this->__method_type, this->__co);
  sf::Renderable::draw(this->__pw_box, this->__pw_cir);
  for(auto& i : this->__pw_key)
    sf::Renderable::draw(i);
}

inline func FakePay::call(const sf::String& str) -> void {
  if(this->isShowing()) return;
  
  sf::Msgable::setLocked(true);
  this->__pwn = 0;
  this->__num.setTextString(str);
  this->__num.resizeByText();
  this->__num.align(sf::Align::T) += {10, 140};
  this->__num_type.align(sf::Align::L) -= {60, 10};
  
  this->__box.setTag<int>(1);
  this->__box.mov.apply(sf::MoveMethod::linear_down({0, -this->__box.getSize().y - 80}, {}));
}

inline func FakePay::isShowing(void) -> bool {
  return
     this->__box.getTag<int>()
  || this->__box_bk.getTag<int>();
}

inline func FakePay::pollEvent(const sf::Event& event) -> bool {
  if(!this->isShowing()) return false;

  if(this->__exit.pollEvent_if(event)) {
     this->__box.mov.apply(sf::MoveMethod::linear_down({0, this->__box.getSize().y + 50}, {}));
     this->__box.setTag<int>(-1);
    sf::Msgable::setLocked(false);
  }
  if(this->__method_type.pollEvent_if(event)) {}
  if(this->__box.getTag<int>()) for(auto& i : this->__pw_key) if(i.pollEvent_if(event)) {
         if(i.getTag<int>() == 11) { if(this->__pwn > 0) --this->__pwn; }
    else if(i.getTag<int>() != 9 && this->__pwn < 6) ++this->__pwn;
    
    if(this->__pwn == 6) {
      this->__box_bk.mov.apply(sf::MoveMethod::linear_down({0, -this->__box.getSize().y - 80}, {}));
      this->__box.mov.apply(sf::MoveMethod::linear_down({0, this->__box.getSize().y + 50}, {}), true);
      this->__box.__ATTRIBUTE__.__VISIBLE_TEXT__ = false;
      this->__box_bk.setTag<int>(1);
      this->__box.   setTag<int>(2);
      this->__exit.__ATTRIBUTE__.__VISIBLE__ = false;
      this->__num.setBuilder(this->__box_bk);
    }
  }
  
  if(this->__box_bk.getTag<int>() == 1 && this->__confirm.pollEvent_if(event)) {
     this->__box_bk.mov.apply(sf::MoveMethod::linear_down({0, this->__box.getSize().y + 50}, {}), true);
     this->__box_bk.setTag<int>(2);
    sf::Msgable::setLocked(false);
    return true;
  } return false;
}
