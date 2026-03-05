#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "strong_type.hpp"

TEST_CASE("Basic strong type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<double, MeterTag>;
  constexpr Meter m1(5.0);
  static_assert(m1.value() == 5.0, "Value should be 5.0");

  SUBCASE("Copy construction")
  {
    constexpr Meter m2 = m1;
    static_assert(m2.value() == 5.0, "Value should be 5.0 after copy");
    CHECK(m2.value() == 5.0);
  }

  SUBCASE("Move construction")
  {
    constexpr Meter m2 = Meter(10.0);
    static_assert(m2.value() == 10.0, "Value should be 10.0 after move");
    CHECK(m2.value() == 10.0);
  }

  SUBCASE("Copy assignment")
  {
    Meter m2(0.0);
    m2 = m1;
    CHECK(m2.value() == 5.0);
  }

  SUBCASE("Move assignment")
  {
    Meter m2(0.0);
    m2 = Meter(15.0);
    CHECK(m2.value() == 15.0);
  }

  SUBCASE("Equality operator")
  {
    constexpr Meter m2(5.0);
    constexpr Meter m3(10.0);
    static_assert(m1 == m2, "m1 should be equal to m2");
    CHECK(m1 == m2);
    static_assert(m1 != m3, "m1 should not be equal to m3");
    CHECK(m1 != m3);
  }

  SUBCASE("Hash function")
  {
    constexpr Meter m2(5.0);
    constexpr Meter m3(10.0);
    CHECK(m1.hash() == m2.hash());
    CHECK(m1.hash() != m3.hash());
  }
}

TEST_CASE("Default constructible strong type")
{
  struct CounterTag
  {
  };
  using Counter = rg::strong_type<int, CounterTag, rg::DefaultConstructible>;
  constexpr Counter c1;
  static_assert(c1.value() == 0, "Default constructed value should be 0");
  CHECK(c1.value() == 0);
}

TEST_CASE("Unitialized constructible strong type")
{
  struct CounterTag
  {
  };
  using Counter =
      rg::strong_type<int, CounterTag, rg::UninitializedConstructible>;
  const Counter c1;
  // Cannot use constexpr since uninitialized values are undefined behavior and
  // cannot be evaluated at compile time
  // constexpr Counter c2;
}

TEST_CASE("Addable type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<double, MeterTag, rg::Addable>;
  constexpr Meter m1(5.0);
  constexpr Meter m2(10.0);
  const auto m3 = m1 + m2;
  CHECK(m3.value() == 15.0);

  SUBCASE("Addition assignment")
  {
    Meter m4(20.0);
    const auto m5 = m4 += m1;
    CHECK(m5.value() == 25.0);
    CHECK(m4.value() == 25.0);
  }
}

TEST_CASE("Subtractable type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<double, MeterTag, rg::Subtractable>;
  constexpr Meter m1(5.0);
  constexpr Meter m2(10.0);
  const auto m3 = m1 - m2;
  CHECK(m3.value() == -5.0);

  SUBCASE("Subtraction assignment")
  {
    Meter m4(20.0);
    const auto m5 = m4 -= m1;
    CHECK(m5.value() == 15.0);
    CHECK(m4.value() == 15.0);
  }
}

TEST_CASE("Multiplyable type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<double, MeterTag, rg::Multiplyable>;
  constexpr Meter m1(5.0);
  constexpr Meter m2(10.0);
  const auto m3 = m1 * m2;
  CHECK(m3.value() == 50.0);

  SUBCASE("Multiplication assignment")
  {
    Meter m4(20.0);
    const auto m5 = m4 *= m1;
    CHECK(m5.value() == 100.0);
    CHECK(m4.value() == 100.0);
  }
}

TEST_CASE("Divisible type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<double, MeterTag, rg::Divisible>;
  constexpr Meter m1(5.0);
  constexpr Meter m2(10.0);
  const auto m3 = m1 / m2;
  CHECK(m3.value() == 0.5);

  SUBCASE("Division assignment")
  {
    Meter m4(20.0);
    const auto m5 = m4 /= m1;
    CHECK(m5.value() == 4.0);
    CHECK(m4.value() == 4.0);
  }
}

TEST_CASE("Arithmetic type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<double, MeterTag, rg::Arithmetic>;
  Meter m1(5.0);
  m1 += Meter(10.0);
  CHECK(m1.value() == 15.0);
  m1 = m1 - Meter(3.0);
  CHECK(m1.value() == 12.0);
  m1 = m1 * Meter(2.0);
  CHECK(m1.value() == 24.0);
  m1 = m1 / Meter(4.0);
  CHECK(m1.value() == 6.0);
}

TEST_CASE("Incrementable type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<uint32_t, MeterTag, rg::Incrementable>;
  Meter m1(5);
  CHECK((++m1).value() == 6);
  CHECK((m1++).value() == 6);
  CHECK(m1.value() == 7);
}

TEST_CASE("Decrementable type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<uint32_t, MeterTag, rg::Decrementable>;
  Meter m1(5);
  CHECK((--m1).value() == 4);
  CHECK((m1--).value() == 4);
  CHECK(m1.value() == 3);
}

TEST_CASE("Orderable type")
{
  struct MeterTag
  {
  };
  using Meter = rg::strong_type<uint32_t, MeterTag, rg::Orderable>;
  Meter m1(5);
  Meter m2(10);
  CHECK(m1 < m2);
  CHECK_FALSE(m1 < m1);
  CHECK_FALSE(m2 < m1);
  CHECK(m1 <= m2);
  CHECK(m1 <= m1);
  CHECK(m2 > m1);
  CHECK(m1 >= m1);
  CHECK(m2 >= m1);
  CHECK_FALSE(m1 > m2);
  CHECK_FALSE(m1 > m1);
}
