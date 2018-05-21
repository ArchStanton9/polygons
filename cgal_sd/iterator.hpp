// iterator.hpp
// Auxiliary metafunctions for iterator types.
#ifndef ITERATOR_HPP_INCLUDED_M0M16Y
#define ITERATOR_HPP_INCLUDED_M0M16Y

#include <iterator>
#include <type_traits>

/// Get a value type of an iterator.
template <class It>
using Iterator_value = typename std::iterator_traits<It>::value_type;

/// Get a difference type of an iterator.
template <class It>
using Iterator_difference = typename std::iterator_traits<It>::difference_type;

/// Get a category tag of an iterator.
template <class It>
using Iterator_tag = typename std::iterator_traits<It>::iterator_category;

/// Check if an iterator is of the given category (tag).
template <class It, class Tag>
using Iterator_tag_is = std::is_convertible<Iterator_tag<It>, Tag>;

template <class It, class Tag>
constexpr bool iterator_tag_is = Iterator_tag_is<It, Tag>::value;

/// Check if an iterator is input.
template <class It>
using Is_input_iterator = Iterator_tag_is<It, std::input_iterator_tag>;

template <class It>
constexpr bool is_input_iterator = Is_input_iterator<It>::value;

/// Check if an iterator is output or a writable input.
template <class It>
using Is_output_iterator = std::bool_constant<
  (Iterator_tag_is<It, std::output_iterator_tag>::value ||
    (Iterator_tag_is<It, std::forward_iterator_tag>::value &&
     !std::is_const_v<Iterator_value<It>>))>;

template <class It>
constexpr bool is_output_iterator = Is_output_iterator<It>::value;

/// Check if an iterator is forward.
template <class It>
using Is_forward_iterator = Iterator_tag_is<It, std::forward_iterator_tag>;

template <class It>
constexpr bool is_forward_iterator = Is_forward_iterator<It>::value;

/// Check if an iterator is bidirectional.
template <class It>
using Is_bidirectional_iterator = Iterator_tag_is<It, std::bidirectional_iterator_tag>;

template <class It>
constexpr bool is_bidirectional_iterator = Is_bidirectional_iterator<It>::value;

/// Check if an iterator is random access.
template <class It>
using Is_random_access_iterator = Iterator_tag_is<It, std::random_access_iterator_tag>;

template <class It>
constexpr bool is_random_access_iterator = Is_random_access_iterator<It>::value;

/// Check if an iterator points to a constant type.
template <class It>
using Is_const_iterator = std::is_const<Iterator_value<It>>;

template <class It>
constexpr bool is_const_iterator = Is_const_iterator<It>::value;

#endif//ITERATOR_HPP_INCLUDED_M0M16Y
