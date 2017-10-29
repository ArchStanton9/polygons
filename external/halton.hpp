// halton.hpp
// quasi-random number generator
// see https://en.wikipedia.org/wiki/Halton_sequence
#ifndef OPENMP_EX_HALTON_HPP_INCLUDED_M49B4Z
#define OPENMP_EX_HALTON_HPP_INCLUDED_M49B4Z
#include <cstdint>
#include <cassert>
#include <cmath>
#include <array>

/// Halton quasi-random sequence generator,
/// Generated numbers belong (0, 1).
template <class Number = double, unsigned Digits = 10>
class Halton
{
  // Sequence parameters.
  unsigned base, stride;

  // Computational state.
  std::array<unsigned, Digits> digits;
  std::array<Number, Digits> weights, addends;
  unsigned active;

  void do_carry()
  {
    for (unsigned i = 1; i < Digits; ++i)
    {
      digits[i-1] = 0;
      addends[i-1] = 0;

      if (++digits[i] != base)
      {
        addends[i] = digits[i] * weights[i];
        if (i == active)
        {
          assert(digits[i] == 1);
          ++active;
        }
        return;
      }
    }

    digits[Digits - 1] = 0;
    addends[Digits - 1] = 0;
    active = 1;
  }

  Number generate_next()
  {
    if (++digits[0] != base)
      addends[0] = digits[0] * weights[0];
    else
      do_carry();

    Number sum = addends[active-1];
    for (unsigned i = active-1; i != 0; --i)
      sum += addends[i-1];
    return sum;
  }

public:
  /// Construct a Halton generator.
  /// @param base is radix, must be 2 or higher
  /// @param stride -- only the last number from each group of size stride is returned
  /// @param start_skip -- how many numbers are skipped in the very beginning
  Halton(int base, int stride, int start_skip)
    : base(base), stride(stride), active(1)
  {
    assert(base >= 2);
    assert(stride >= 1);
    assert(start_skip >= 0);
    using std::pow;

    // Prepare computational state.
    digits.fill(0);
    addends.fill(Number(0));

    for (unsigned digit = 0; digit < Digits; ++digit)
      weights[digit] = Number(1) / pow(Number(base), digit+1);

    // Apply start_skip.
    while (start_skip-- > 0)
      generate_next();
  }

  /// Start skip == base + 1.
  explicit Halton(int base, int stride = 1)
    : Halton(base, stride, base + 1) {}

  /// Generate the next number.
  Number operator()()
  {
    for (unsigned s = 1; s < stride; ++s)
      generate_next();
    return generate_next();
  }
};

#endif//OPENMP_EX_HALTON_HPP_INCLUDED_M49B4Z
