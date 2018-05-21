// bit_flag_type.hpp
// Introduce bit flag types.
#ifndef BIT_FLAG_TYPE_HPP_INCLUDED_1NW73K
#define BIT_FLAG_TYPE_HPP_INCLUDED_1NW73K

#include <initializer_list>
#include <type_traits>

/// It is supposed that FlagType is a enum or an integer type.
/// Bit_flags is based upon std::bitset, yet somewhat simplified.
template <class FlagType>
class Bit_flags
{
public:
  static_assert(sizeof(FlagType) <= sizeof(long long));
  static_assert(std::is_integral_v<FlagType> || std::is_enum_v<FlagType>);
  
  using Base = std::conditional_t< // If 
    std::is_integral_v<FlagType>,     // FlagType is integral
          std::make_unsigned_t<FlagType>,    // then unsigned version of itself,
          std::underlying_type_t<FlagType>>; // the underlying integer type otherwise.
  
private:
  Base _data;
  
  constexpr static Base _bit(FlagType flag) noexcept
  {
    return Base(1) << static_cast<Base>(flag);
  }
  
  constexpr static Base _make_data(std::initializer_list<FlagType> flags) noexcept
  {
    Base result = 0;
    for (auto flag : flags)
      result |= _bit(flag);
    return result;
  }
  
  constexpr Bit_flags(Base data, int) noexcept
    : _data(data) {}
  
public:
  constexpr Bit_flags() noexcept: _data(0) {}
  constexpr Bit_flags(Bit_flags const &) noexcept = default;
  constexpr Bit_flags& operator=(Bit_flags const &) noexcept = default;
  
  constexpr Bit_flags(FlagType flag) noexcept
    : _data(_bit(flag)) {}
  constexpr Bit_flags(std::initializer_list<FlagType> flags) noexcept
    : _data(_make_data(flags)) {}
  
  
  constexpr Base data() const noexcept
  {
    return _data;
  }
  
  constexpr static Bit_flags data(Base value) noexcept
  {
    return Bit_flags(value, 0);
  }
  
  
  constexpr bool operator[](FlagType flag) const noexcept
  {
    return (_data & _bit(flag)) != 0;
  }
  
  constexpr bool any() const noexcept
  {
    return _data != 0;
  }
  
  constexpr bool none() const noexcept
  {
    return _data == 0;
  }
  
  constexpr Bit_flags& operator&=(Bit_flags const & other) noexcept
  {
    _data &= other._data;
    return *this;
  }
  
  constexpr Bit_flags& operator|=(Bit_flags const & other) noexcept
  {
    _data |= other._data;
    return *this;
  }
  
  constexpr Bit_flags& operator^=(Bit_flags const & other) noexcept
  {
    _data ^= other._data;
    return *this;
  }
    
  constexpr Bit_flags& operator~() const noexcept
  {
    return data(~_data);
  }
};


template <class FlagType>
constexpr inline bool operator==
  (Bit_flags<FlagType> const & a, Bit_flags<FlagType> const & b) noexcept
{
  return a.data() == b.data();
} 

template <class FlagType>
constexpr inline bool operator!=
  (Bit_flags<FlagType> const & a, Bit_flags<FlagType> const & b) noexcept
{
  return !(a == b);
}

template <class FlagType>
constexpr inline Bit_flags<FlagType> operator&
  (Bit_flags<FlagType> const & a, Bit_flags<FlagType> const & b) noexcept
{
  return Bit_flags<FlagType>(a) &= b;
}

template <class FlagType>
constexpr inline Bit_flags<FlagType> operator|
  (Bit_flags<FlagType> const & a, Bit_flags<FlagType> const & b) noexcept
{
  return Bit_flags<FlagType>(a) |= b;
}

template <class FlagType>
constexpr inline Bit_flags<FlagType> operator^
  (Bit_flags<FlagType> const & a, Bit_flags<FlagType> const & b) noexcept
{
  return Bit_flags<FlagType>(a) ^= b;
}

#endif//BIT_FLAG_TYPE_HPP_INCLUDED_1NW73K
