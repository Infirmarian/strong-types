#ifndef STRONG_TYPE_HPP
#define STRONG_TYPE_HPP

#include <array>
#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201703L
#define RG_NODISCARD [[nodiscard]]
#define RG_CONSTEXPR_IF_17 constexpr
#else
#define RG_NODISCARD
#define RG_CONSTEXPR_IF_17
#endif
#if __cplusplus >= 201402L
#define RG_CONSTEXPR_IF_14 constexpr
#else
#define RG_CONSTEXPR_IF_14
#endif

namespace rg
{
/// @brief Trait that allows addition operations on the strong type.
template <typename StrongType>
struct Addable
{
  RG_NODISCARD friend constexpr StrongType operator+(const StrongType& lhs,
                                                     const StrongType& rhs)
  {
    return StrongType{lhs.value() + rhs.value()};
  }

  RG_CONSTEXPR_IF_14 StrongType& operator+=(const StrongType& other)
  {
    auto& self = static_cast<StrongType&>(*this);
    self.value() += other.value();
    return self;
  }
};

/// @brief Trait that allows subtraction operations on the strong type.
template <typename StrongType>
struct Subtractable
{
  RG_NODISCARD friend constexpr StrongType operator-(const StrongType& lhs,
                                                     const StrongType& rhs)
  {
    return StrongType{lhs.value() - rhs.value()};
  }

  RG_CONSTEXPR_IF_14 StrongType& operator-=(const StrongType& other)
  {
    auto& self = static_cast<StrongType&>(*this);
    self.value() -= other.value();
    return self;
  }
};

/// @brief Trait that allows multiplication operations on the strong type.
template <typename StrongType>
struct Multiplyable
{
  RG_NODISCARD friend constexpr StrongType operator*(const StrongType& lhs,
                                                     const StrongType& rhs)
  {
    return StrongType{lhs.value() * rhs.value()};
  }

  RG_CONSTEXPR_IF_14 StrongType& operator*=(const StrongType& other)
  {
    auto& self = static_cast<StrongType&>(*this);
    self.value() *= other.value();
    return self;
  }
};

/// @brief Trait that allows division operations on the strong type.
template <typename StrongType>
struct Divisible
{
  RG_NODISCARD friend constexpr StrongType operator/(const StrongType& lhs,
                                                     const StrongType& rhs)
  {
    return StrongType{lhs.value() / rhs.value()};
  }

  RG_CONSTEXPR_IF_14 StrongType& operator/=(const StrongType& other)
  {
    auto& self = static_cast<StrongType&>(*this);
    self.value() /= other.value();
    return self;
  }
};

/// @brief Composite trait that grants all arithmetic operations to the strong
/// type.
template <typename StrongType>
struct Arithmetic : Multiplyable<StrongType>,
                    Divisible<StrongType>,
                    Subtractable<StrongType>,
                    Addable<StrongType>
{
};

/// @brief Trait that allows increment operations on the strong type.
template <typename StrongType>
struct Incrementable
{
  RG_CONSTEXPR_IF_14 StrongType operator++(int)
  {
    auto& self = static_cast<StrongType&>(*this);
    auto temp = self;
    self.value()++;
    return temp;
  }

  RG_CONSTEXPR_IF_14 StrongType& operator++()
  {
    auto& self = static_cast<StrongType&>(*this);
    ++self.value();
    return self;
  }
};

/// @brief Trait that allows decrement operations on the strong type.
template <typename StrongType>
struct Decrementable
{
  RG_CONSTEXPR_IF_14 StrongType operator--(int)
  {
    auto& self = static_cast<StrongType&>(*this);
    auto temp = self;
    self.value()--;
    return temp;
  }

  RG_CONSTEXPR_IF_14 StrongType& operator--()
  {
    auto& self = static_cast<StrongType&>(*this);
    --self.value();
    return self;
  }
};

/// @brief Trait that allows ordering comparisons of the strong type.
template <typename StrongType>
struct Orderable
{
  RG_NODISCARD friend constexpr bool operator<(const StrongType& lhs,
                                               const StrongType& rhs)
  {
    return lhs.value() < rhs.value();
  }
  RG_NODISCARD friend constexpr bool operator>(const StrongType& lhs,
                                               const StrongType& rhs)
  {
    return lhs.value() > rhs.value();
  }
  RG_NODISCARD friend constexpr bool operator<=(const StrongType& lhs,
                                                const StrongType& rhs)
  {
    return lhs.value() <= rhs.value();
  }
  RG_NODISCARD friend constexpr bool operator>=(const StrongType& lhs,
                                                const StrongType& rhs)
  {
    return lhs.value() >= rhs.value();
  }
};

/// @brief Trait that allows default construction of the strong type. Without
/// this trait specified, all instances of the type must be constructed with an
/// explicit value.
template <typename StrongType>
struct DefaultConstructible
{
};

/// @brief Trait that allows uninitialized default construction of the strong
/// type. This may be useful for large objects where default initialization
/// would be cost prohibitive.
template <typename StrongType>
struct UninitializedConstructible
{
};

/// @brief A strong type wrapper that creates a distinct type from an underlying
/// type T, and has optionally specified traits.
/// @tparam T The underlying type to be wrapped.
/// @tparam Tag A unique tag type to differentiate this strong type from others
/// with the same underlying type. This typically can just be an empty struct.
/// @tparam Traits A variadic list of trait templates to be applied to the
/// strong type
template <typename T, typename Tag, template <typename> class... Traits>
class strong_type : public Traits<strong_type<T, Tag, Traits...>>...
{
 public:
  using underlying_type = T;
  using type_tag = Tag;

 private:
  T _value;

 public:
  template <
      typename U = strong_type,
      typename std::enable_if<
          std::is_base_of<DefaultConstructible<U>, U>::value, int>::type = 0>
  constexpr strong_type() : _value{}
  {
  }

  template <typename U = strong_type,
            typename std::enable_if<
                std::is_base_of<UninitializedConstructible<U>, U>::value,
                int>::type = 0>
  constexpr strong_type()
  {
  }

  constexpr explicit strong_type(const T& value) : _value(value) {}
  constexpr explicit strong_type(T&& value) : _value(std::move(value)) {}

  /// @brief Access the underlying value as a weak type
  RG_NODISCARD RG_CONSTEXPR_IF_14 T& value() { return _value; }

  /// @brief Access the underlying value as a weak type
  RG_NODISCARD constexpr const T& value() const { return _value; }

  /// @brief Equality operator
  RG_NODISCARD constexpr bool operator==(const strong_type& other) const
  {
    return _value == other._value;
  }

  /// @brief Inequality operator
  RG_NODISCARD constexpr bool operator!=(const strong_type& other) const
  {
    return !(_value == other._value);
  }

  /// @brief Hash function for the underlying type
  RG_NODISCARD constexpr std::size_t hash() const
  {
    return std::hash<T>{}(_value);
  }
};
namespace detail
{
template <int Meter, int Second>
struct Dimension
{
  constexpr static int meter_exp = Meter;
  constexpr static int second_exp = Second;
};

template <typename DimLHS, typename DimRHS>
struct DimMultiply
{
  using type = Dimension<DimLHS::meter_exp + DimRHS::meter_exp,
                         DimLHS::second_exp + DimRHS::second_exp>;
};

template <typename DimLHS, typename DimRHS>
struct DimDivide
{
  using type = Dimension<DimLHS::meter_exp - DimRHS::meter_exp,
                         DimLHS::second_exp - DimRHS::second_exp>;
};

using DimLength = Dimension<1, 0>;
using DimTime = Dimension<0, 1>;
using DimScalar = Dimension<0, 0>;
inline RG_CONSTEXPR_IF_14 std::size_t int_to_string(char* buffer, int value)
{
  std::array<char, 20> tempBuffer{};
  std::size_t index = 0;
  const bool isNegative = value < 0;
  if (isNegative)
  {
    buffer[0] = '-';
    value = -value;
    buffer += 1;
  }
  do
  {
    tempBuffer[index++] = '0' + (value % 10);
    value /= 10;
  } while (value != 0);
  for (std::size_t i = 0; i < index; ++i)
  {
    buffer[i] = tempBuffer[index - i - 1];
  }
  return index + (isNegative ? 1 : 0);
}

template <typename DimT>
RG_CONSTEXPR_IF_14 auto dimension_label_array_raw() -> std::array<char, 1000>
{
  auto result = std::array<char, 1000>{};
  auto index = 0;
  if (DimT::meter_exp != 0)
  {
    result[index++] = ' ';
    result[index++] = 'm';
    if (DimT::meter_exp != 1)
    {
      result[index++] = '^';
      index += detail::int_to_string(&result[index], DimT::meter_exp);
    }
  }
  if (DimT::second_exp != 0)
  {
    result[index++] = ' ';
    result[index++] = 's';
    if (DimT::second_exp != 1)
    {
      result[index++] = '^';
      index += detail::int_to_string(&result[index], DimT::second_exp);
    }
  }
  return result;
}

template <std::size_t N>
RG_CONSTEXPR_IF_14 std::size_t dimension_label_array_length(
    const std::array<char, N>& raw)
{
  std::size_t length = 0;
  while (length < raw.size() && raw[length] != '\0')
  {
    ++length;
  }
  return length;
}
}  // namespace detail

/// @brief Strongly typed quantity with associated dimension tag.
/// @tparam DimT The dimension tag type.
template <typename DimT>
using quantity = strong_type<double, DimT, Addable, Subtractable>;

/// @brief Meter dimension strong type.
using Meters = quantity<detail::DimLength>;
/// @brief Second dimension strong type.
using Seconds = quantity<detail::DimTime>;

namespace literals
{
constexpr Meters operator""_m(long double value)
{
  return Meters{static_cast<double>(value)};
}

constexpr Seconds operator""_s(long double value)
{
  return Seconds{static_cast<double>(value)};
}

constexpr Meters operator""_m(unsigned long long value)
{
  return Meters{static_cast<double>(value)};
}

constexpr Seconds operator""_s(unsigned long long value)
{
  return Seconds{static_cast<double>(value)};
}
}  // namespace literals

/// @brief Multiplication operator for quantities.
template <typename DimLHS, typename DimRHS>
auto operator*(const quantity<DimLHS>& lhs, const quantity<DimRHS>& rhs)
    -> quantity<typename detail::DimMultiply<DimLHS, DimRHS>::type>
{
  return quantity<typename detail::DimMultiply<DimLHS, DimRHS>::type>{
      lhs.value() * rhs.value()};
}

/// @brief Division operator for quantities.
template <typename DimLHS, typename DimRHS>
auto operator/(const quantity<DimLHS>& lhs, const quantity<DimRHS>& rhs)
    -> quantity<typename detail::DimDivide<DimLHS, DimRHS>::type>
{
  return quantity<typename detail::DimDivide<DimLHS, DimRHS>::type>{
      lhs.value() / rhs.value()};
}

/// C++17 constexpr can deduce array sizes at compile time, not supported in
/// C++11/14
#if __cplusplus >= 201703L
template <typename DimT>
constexpr auto dimension_label_array()
{
  constexpr auto raw = detail::dimension_label_array_raw<DimT>();
  constexpr auto length = detail::dimension_label_array_length(raw);
  auto result = std::array<char, length + 1>{};
  for (std::size_t i = 0; i < length; ++i)
  {
    result[i] = raw[i];
  }
  return result;
}
#else
template <typename DimT>
auto dimension_label_array() -> std::array<char, 1000>
{
  auto raw = detail::dimension_label_array_raw<DimT>();
  return raw;
}
#endif

template <typename DimT>
std::string to_string(const quantity<DimT>& qty)
{
  auto result =
      std::to_string(qty.value()) + dimension_label_array<DimT>().data();
  return result;
}

template <typename DimT>
std::ostream& operator<<(std::ostream& os, const quantity<DimT>& qty)
{
  os << qty.value() << dimension_label_array<DimT>().data();
  return os;
}
}  // namespace rg

#endif  // STRONG_TYPE_HPP
