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

template <typename StrongType>
struct Arithmetic : Multiplyable<StrongType>,
                    Divisible<StrongType>,
                    Subtractable<StrongType>,
                    Addable<StrongType>
{
};

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

template <typename StrongType>
struct DefaultConstructible
{
};

template <typename StrongType>
struct UninitializedConstructible
{
};

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
  RG_NODISCARD RG_CONSTEXPR_MUTABLE T& value() { return _value; }
  RG_NODISCARD constexpr const T& value() const { return _value; }

  RG_NODISCARD constexpr bool operator==(const strong_type& other) const
  {
    return _value == other._value;
  }
  RG_NODISCARD constexpr bool operator!=(const strong_type& other) const
  {
    return !(_value == other._value);
  }
  RG_NODISCARD constexpr std::size_t hash() const
  {
    return std::hash<T>{}(_value);
  }
};
}  // namespace rg

#endif  // STRONG_TYPE_HPP
