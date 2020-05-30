#include <xu/core/widget.hpp>

class CustomWidget : public xu::Widget {
public:
  xu::FSize2 SizeHint() const override {
    return xu::FSize2{1.0f, 1.0f};
  }
};

int main() {
  CustomWidget pog;
  return 0;
}