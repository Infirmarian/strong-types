#ifndef STRONG_TYPE_HPP
#define STRONG_TYPE_HPP

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201703L
#define RG_NODISCARD [[nodiscard]]
#else
#define RG_NODISCARD
#endif
#if __cplusplus >= 201402L
#define RG_CONSTEXPR_MUTABLE constexpr
#else
#define RG_CONSTEXPR_MUTABLE
#endif

namespace rg
{
namespace detail
{
template <typename StrongType, typename Trait>
struct CRTP_Helper
{
 public:
  constexpr const StrongType& derived() const
  {
    return static_cast<const StrongType&>(*this);
  }
  RG_CONSTEXPR_MUTABLE StrongType& derived()
  {
    return static_cast<StrongType&>(*this);
  }
};
}  // namespace detail

/// @brief Trait that allows addition operations on the strong type.
template <typename StrongType>
struct Addable : detail::CRTP_Helper<StrongType, Addable<StrongType>>
{
  RG_NODISCARD constexpr StrongType operator+(const StrongType& other) const
  {
    return StrongType{this->derived().value() + other.value()};
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator+=(const StrongType& other)
  {
    this->derived().value() += other.value();
    return this->derived();
  }
};

/// @brief Trait that allows subtraction operations on the strong type.
template <typename StrongType>
struct Subtractable : detail::CRTP_Helper<StrongType, Subtractable<StrongType>>
{
  RG_NODISCARD constexpr StrongType operator-(const StrongType& other) const
  {
    return StrongType{this->derived().value() - other.value()};
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator-=(const StrongType& other)
  {
    this->derived().value() -= other.value();
    return this->derived();
  }
};

/// @brief Trait that allows multiplication operations on the strong type.
template <typename StrongType>
struct Multiplyable : detail::CRTP_Helper<StrongType, Multiplyable<StrongType>>
{
  RG_NODISCARD constexpr StrongType operator*(const StrongType& other) const
  {
    return StrongType{this->derived().value() * other.value()};
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator*=(const StrongType& other)
  {
    this->derived().value() *= other.value();
    return this->derived();
  }
};

/// @brief Trait that allows division operations on the strong type.
template <typename StrongType>
struct Divisible : detail::CRTP_Helper<StrongType, Divisible<StrongType>>
{
  RG_NODISCARD constexpr StrongType operator/(const StrongType& other) const
  {
    return StrongType{this->derived().value() / other.value()};
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator/=(const StrongType& other)
  {
    this->derived().value() /= other.value();
    return this->derived();
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
    : detail::CRTP_Helper<StrongType, Incrementable<StrongType>>
{
  RG_CONSTEXPR_MUTABLE StrongType operator++(int)
  {
    StrongType temp = this->derived();
    this->derived().value()++;
    return temp;
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator++()
  {
    ++this->derived().value();
    return this->derived();
  }
};

/// @brief Trait that allows decrement operations on the strong type.
template <typename StrongType>
struct Decrementable
    : detail::CRTP_Helper<StrongType, Decrementable<StrongType>>
{
  RG_CONSTEXPR_MUTABLE StrongType operator--(int)
  {
    StrongType temp = this->derived();
    this->derived().value()--;
    return temp;
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator--()
  {
    --this->derived().value();
    return this->derived();
  }
};

/// @brief Trait that allows ordering comparisons of the strong type.
template <typename StrongType>
struct Orderable : detail::CRTP_Helper<StrongType, Orderable<StrongType>>
{
  RG_NODISCARD constexpr bool operator<(const StrongType& other) const
  {
    return this->derived().value() < other.value();
  }
  RG_NODISCARD constexpr bool operator>(const StrongType& other) const
  {
    return this->derived().value() > other.value();
  }
  RG_NODISCARD constexpr bool operator<=(const StrongType& other) const
  {
    return this->derived().value() <= other.value();
  }
  RG_NODISCARD constexpr bool operator>=(const StrongType& other) const
  {
    return this->derived().value() >= other.value();
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
  RG_NODISCARD RG_CONSTEXPR_MUTABLE T& value() { return _value; }

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
}  // namespace rg

#endif  // STRONG_TYPE_HPP
