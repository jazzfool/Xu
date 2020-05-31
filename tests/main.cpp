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
  IVector2 sumS1 = b + s;
  IVector2 sumS2 = s + b;
  IVector2 subS1 = b - s;
  IVector2 subS2 = s - b;
  IVector2 multS1 = b * s;
  IVector2 multS2 = s * b;
  IVector2 divS1 = b / s;
  IVector2 divS2 = s / b;
  assert(sumS1.x == b.x + s && sumS1.y == b.y + s);
  assert(subS1.x == b.x - s && subS1.y == b.y - s);
  assert(multS1.x == b.x * s && multS1.y == b.y * s);
  assert(divS1.x == b.x / s && divS1.y == b.y / s);
  assert(sumS1 == sumS2);
  assert(subS1 == subS2);
  assert(multS1 == multS2);
  assert(divS1 == divS2);

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