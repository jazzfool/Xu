#include "xu/core/Bounds2.hpp"
#include "xu/core/Point2.hpp"
#include "xu/core/Vector2.hpp"
#include <assert.h>
#include <initializer_list>
#include <xu/core/Widget.hpp>
#include <xu/core/Rect2.hpp>

class CustomWidget : public xu::Widget {
public:
    xu::FSize2 SizeHint() const override { return xu::FSize2{1.0f, 1.0f}; }
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

void TestBounds2() {
    using xu::IBounds2;
    using xu::IVector2;
    using xu::IPoint2;

    IBounds2 bigBoi(IPoint2(-10, -5), IPoint2(20, 8));
    IBounds2 smallBoi(IPoint2(-2, -1), IPoint2(3, 4));
    IBounds2 farBoi(IPoint2(100, 100), IPoint2(110, 105));

    assert(bigBoi.lower == IPoint2(-10, -5) && bigBoi.upper == IPoint2(20, 8));
    assert(bigBoi.Top() == bigBoi.lower.y && bigBoi.Bottom() == bigBoi.upper.y);
    assert(bigBoi.Left() == bigBoi.lower.x && bigBoi.Right() == bigBoi.upper.x);

    assert(bigBoi.FullyContains(smallBoi) && bigBoi.Overlaps(smallBoi));
    assert(!smallBoi.FullyContains(bigBoi) && smallBoi.Overlaps(bigBoi));

    assert(!bigBoi.Overlaps(farBoi) && !smallBoi.Overlaps(farBoi));
    assert(!farBoi.Overlaps(bigBoi) && !farBoi.Overlaps(smallBoi));
    assert(!bigBoi.FullyContains(farBoi) && !smallBoi.FullyContains(farBoi));
    assert(!farBoi.FullyContains(bigBoi) && !farBoi.FullyContains(smallBoi));

    assert(bigBoi.ContainsPoint(IPoint2(1, 2))
        && !bigBoi.ContainsPoint(IPoint2(40, 50)));

    IBounds2 grower(IPoint2(-20, -10), IPoint2(20, 10));
    auto fattened = grower.Fattened(-0.5f);

    assert(fattened.lower == grower.lower / 2.f
        && fattened.upper == grower.upper / 2.f);
    assert(grower.Size() == 2 * fattened.Size());

    std::initializer_list<IPoint2> vertices
        = {{12, 20}, {0, -5}, {100, 0}, {-3, 35}};
    IBounds2 vBounds = IBounds2(vertices);

    assert(
        vBounds.lower == IPoint2(-3, -5) && vBounds.upper == IPoint2(100, 35));

    printf("Bounds test complete!\n");
}

void TestRect2() {
    using xu::IRect2;
    using xu::IPoint2;
    using xu::IVector2;

    IRect2 bigBoi(IPoint2(-10, -5), IVector2(30, 13));
    IRect2 smallBoi(IPoint2(-2, -1), IVector2(5, 5));
    IRect2 farBoi(IPoint2(100, 100), IVector2(10, 5));

    assert(bigBoi.origin == IPoint2(-10, -5) && bigBoi.size == IPoint2(30, 13));
    assert(bigBoi.Bounds().lower == bigBoi.origin
        && bigBoi.Bounds().upper == bigBoi.origin + bigBoi.size);
    assert(IRect2(bigBoi.Bounds()) == bigBoi);
    assert(bigBoi.Bounds().Size() == bigBoi.size);

    assert(bigBoi.FullyContains(smallBoi) && bigBoi.Overlaps(smallBoi));
    assert(!smallBoi.FullyContains(bigBoi) && smallBoi.Overlaps(bigBoi));

    assert(!bigBoi.Overlaps(farBoi) && !smallBoi.Overlaps(farBoi));
    assert(!farBoi.Overlaps(bigBoi) && !farBoi.Overlaps(smallBoi));
    assert(!bigBoi.FullyContains(farBoi) && !smallBoi.FullyContains(farBoi));
    assert(!farBoi.FullyContains(bigBoi) && !farBoi.FullyContains(smallBoi));

    printf("Rect test complete!\n");
}

int main() {
    // CustomWidget pog;

    TestVector2();
    TestBounds2();
    TestRect2();

    return 0;
}