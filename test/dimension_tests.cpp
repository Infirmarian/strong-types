#include "doctest.h"
#include "strong_type.hpp"

using namespace rg::literals;

TEST_CASE("Dimensional Addition and subtractions")
{
  auto length1 = 100_m;
  auto length2 = 50_m;
  SUBCASE("Dimensional Addition")
  {
    auto totalLength = length1 + length2;
    CHECK(totalLength.value() == 150);
    static_assert(std::is_same<decltype(totalLength), rg::Meters>::value,
                  "Resulting type should be Meters");
  }
  SUBCASE("Dimensional Subtraction")
  {
    auto totalLength = length1 - length2;
    CHECK(totalLength.value() == 50);
    static_assert(std::is_same<decltype(totalLength), rg::Meters>::value,
                  "Resulting type should be Meters");
  }
}

TEST_CASE("Dimensional multiplication")
{
  auto length1 = 100_m;
  auto length2 = 50_m;
  SUBCASE("meters squared")
  {
    auto area = length1 * length2;
    CHECK(area.value() == 5000);
    static_assert(decltype(area)::type_tag::meter_exp == 2,
                  "Area should have meter exponent 2");
    static_assert(decltype(area)::type_tag::second_exp == 0,
                  "Area should have second exponent 0");
  }

  SUBCASE("meter seconds")
  {
    auto meter_seconds = length1 * 10_s;
    CHECK(meter_seconds.value() == 1000);
    static_assert(decltype(meter_seconds)::type_tag::meter_exp == 1,
                  "meter_seconds should have meter exponent 1");
    static_assert(decltype(meter_seconds)::type_tag::second_exp == 1,
                  "meter_seconds should have second exponent 1");
  }
}

TEST_CASE("Dimensional division")
{
  auto length1 = 100_m;
  auto length2 = 50_m;
  SUBCASE("meters to scalar")
  {
    auto ratio = length1 / length2;
    CHECK(ratio.value() == 2);
    static_assert(decltype(ratio)::type_tag::meter_exp == 0,
                  "Ratio should have meter exponent 0");
    static_assert(decltype(ratio)::type_tag::second_exp == 0,
                  "Ratio should have second exponent 0");
  }

  SUBCASE("meters per second")
  {
    auto meters_per_second = length1 / 10_s;
    CHECK(meters_per_second.value() == 10);
    static_assert(decltype(meters_per_second)::type_tag::meter_exp == 1,
                  "meters_per_second should have meter exponent 1");
    static_assert(decltype(meters_per_second)::type_tag::second_exp == -1,
                  "meter_seconds should have second exponent -1");
  }
}

TEST_CASE("Dimensional Label Arrays")
{
  SUBCASE("Length dimension label")
  {
    using LengthDim = rg::detail::Dimension<1, 0>;
    RG_CONSTEXPR_IF_17 auto lengthLabelArray =
        rg::dimension_label_array<LengthDim>();
    CHECK(std::string(lengthLabelArray.data()) == " m");
  }
  SUBCASE("Area dimension label")
  {
    using LengthDim = rg::detail::Dimension<2, 0>;
    RG_CONSTEXPR_IF_17 auto lengthLabelArray =
        rg::dimension_label_array<LengthDim>();
    CHECK(std::string(lengthLabelArray.data()) == " m^2");
  }
  SUBCASE("Time dimension label")
  {
    using TimeDim = rg::detail::Dimension<0, 1>;
    RG_CONSTEXPR_IF_17 auto timeLabelArray =
        rg::dimension_label_array<TimeDim>();
    CHECK(std::string(timeLabelArray.data()) == " s");
  }
  SUBCASE("Velocity dimension label")
  {
    using VelocityDim = rg::detail::Dimension<1, -1>;
    RG_CONSTEXPR_IF_17 auto velocityLabelArray =
        rg::dimension_label_array<VelocityDim>();
    CHECK(std::string(velocityLabelArray.data()) == " m s^-1");
  }
  SUBCASE("Acceleration dimension label")
  {
    using AccelerationDim = rg::detail::Dimension<1, -2>;
    RG_CONSTEXPR_IF_17 auto accelerationLabelArray =
        rg::dimension_label_array<AccelerationDim>();
    CHECK(std::string(accelerationLabelArray.data()) == " m s^-2");
  }
  SUBCASE("Area per second squared label")
  {
    using LengthDim = rg::detail::Dimension<2, -5>;
    RG_CONSTEXPR_IF_17 auto lengthLabelArray =
        rg::dimension_label_array<LengthDim>();
    CHECK(std::string(lengthLabelArray.data()) == " m^2 s^-5");
  }
}

TEST_CASE("ToString for dimensions")
{
  auto length = 100_m;
  SUBCASE("Scalar to string")
  {
    auto scalar = rg::quantity<rg::detail::DimScalar>{42};
    auto scalarStr = rg::to_string(scalar);
    CHECK(scalarStr == "42.000000");
  }
  SUBCASE("Length to string")
  {
    auto lengthStr = rg::to_string(length);
    CHECK(lengthStr == "100.000000 m");
  }
  SUBCASE("Scalar to string")
  {
    auto scalar = length / length;
    auto scalarStr = rg::to_string(scalar);
    CHECK(scalarStr == "1.000000");
  }
  SUBCASE("Velocity to string")
  {
    auto velocity = length / 10_s;
    auto velocityStr = rg::to_string(velocity);
    CHECK(velocityStr == "10.000000 m s^-1");
  }
  SUBCASE("Acceleration to string")
  {
    auto acc = length / 10_s / 10_s;
    auto accStr = rg::to_string(acc);
    CHECK(accStr == "1.000000 m s^-2");
  }
}
