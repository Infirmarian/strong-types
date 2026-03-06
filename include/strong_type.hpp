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
/// @brief Trait that allows addition operations on the strong type.
template <typename StrongType>
struct Addable
{
  RG_NODISCARD friend constexpr StrongType operator+(const StrongType& lhs,
                                                     const StrongType& rhs)
  {
    return StrongType{lhs.value() + rhs.value()};
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator+=(const StrongType& other)
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

  RG_CONSTEXPR_MUTABLE StrongType& operator-=(const StrongType& other)
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

  RG_CONSTEXPR_MUTABLE StrongType& operator*=(const StrongType& other)
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

  RG_CONSTEXPR_MUTABLE StrongType& operator/=(const StrongType& other)
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
  RG_CONSTEXPR_MUTABLE StrongType operator++(int)
  {
    auto& self = static_cast<StrongType&>(*this);
    auto temp = self;
    self.value()++;
    return temp;
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator++()
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
  RG_CONSTEXPR_MUTABLE StrongType operator--(int)
  {
    auto& self = static_cast<StrongType&>(*this);
    auto temp = self;
    self.value()--;
    return temp;
  }

  RG_CONSTEXPR_MUTABLE StrongType& operator--()
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
