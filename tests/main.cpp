#include <assert.h>
#include <xu/core/Widget.hpp>

class CustomWidget : public xu::Widget {
public:
  xu::FSize2 SizeHint() const override {
    return xu::FSize2{1.0f, 1.0f};
  }
};

void TestVector2() {
  using xu::IVector2;
  using xu::FVector2;
  
  IVector2 a(2, 5);
  IVector2 b{3, 6};
  IVector2 c = IVector2(4, 7);

  assert(a.x == 2 && a.y == 5);
  assert(b.x == 3 && b.y == 6);
  assert(c.x == 4 && c.y == 7);

  IVector2 sumAB = a + b;
  IVector2 subAB = a - b;
  IVector2 multAB = a * b;
  IVector2 divAB = a / b;
  assert(sumAB.x == a.x + b.x && sumAB.y == a.y + b.y);
  assert(subAB.x == a.x - b.x && subAB.y == a.y - b.y);
  assert(multAB.x == a.x * b.x && multAB.y == a.y * b.y);
  assert(divAB.x == a.x / b.x && divAB.y == a.y / b.y);

  IVector2 negA = -a;
  assert(negA.x == -a.x && negA.y == -a.y);

  int s = 3;
  IVector2 sumS = b + s;
  IVector2 subS = b - s;
  IVector2 multS = b * s;
  IVector2 divS = b / s;
  assert(sumS.x == b.x + s && sumS.y == b.y + s);
  assert(subS.x == b.x - s && subS.y == b.y - s);
  assert(multS.x == b.x * s && multS.y == b.y * s);
  assert(divS.x == b.x / s && divS.y == b.y / s);

  assert(a == a);
  assert(a != b);

  FVector2 f1{3.0f, 5.2f};
  FVector2 f2{3.0f, 5.3f};
  FVector2 f3{1.9f, 5.2f};
  FVector2 f4{3.0f, 5.2f};
  assert(f1 == f4);
  assert(f1 != f2 && f1 != f3 && f2 != f3);


  printf("Vector test complete!\n");
}

int main() {
  CustomWidget pog;

  printf("Hello World Test\n");
  TestVector2();

  return 0;
}