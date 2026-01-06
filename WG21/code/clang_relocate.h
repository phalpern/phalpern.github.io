/* clang_relocate.h                                                   -*-C++-*-
 *
 * Copyright (C) 2025 Pablo Halpern <phalpern@halpernwightsoftware.com>
 * Distributed under the Boost Software License - Version 1.0
 */

// Implementation of `is_trivially_relocatable`, `is_replaceable`,
// `is_nothrow_relocatable`, `trivially_relocate()`, and `relocate()` using the
// Clang built-in traits and functions.

#ifndef INCLUDED_CLANG_RELOCATE
#define INCLUDED_CLANG_RELOCATE

#include <type_traits>
#include <utility>
#include <new>
#include <memory>

namespace std {

// TRAITS

template <class T>
struct is_trivially_relocatable :
    bool_constant<__builtin_is_cpp_trivially_relocatable(T)> {};

template <class T>
constexpr inline bool
is_trivially_relocatable_v = is_trivially_relocatable<T>::value;

template <class T>
struct is_replaceable : bool_constant<__builtin_is_replaceable(T)> {};

template <class T>
constexpr inline bool is_replaceable_v = is_replaceable<T>::value;

template <class T>
struct is_nothrow_relocatable :
    bool_constant<(is_trivially_relocatable_v<T> ||
                   is_nothrow_move_constructible_v<remove_all_extents_t<T>> &&
                   is_destructible_v<remove_all_extents_t<T>>)>
{};

template <class T>
constexpr inline bool
is_nothrow_relocatable_v = is_nothrow_relocatable<T>::value;


// `std::trivially_relocate()`

template <class T>
inline T* trivially_relocate(T* first, T* last, T* result)
{
  static_assert(is_trivially_relocatable_v<T>);
  return __builtin_trivially_relocate(result, first, last-first);
}

// `std::relocate()`

// Forward declaration
template <class T>
constexpr T* relocate(T* first, T* last, T* result) noexcept;

/// `constexpr`-friendly pointer-in-range test.
template <class T>
constexpr
bool __ptr_is_in_range(const T* p, const T* beg, const T* end) noexcept
{
  if consteval {
    // Cannot compare unrelated pointers in consteval, so linearly check range.
    for (; beg != end; ++beg)
      if (p == beg)
        return true;
    return false;
  }
  else {
    return (beg <= p && p < end);
  }
}

template <class T>
constexpr void __move_destroy_element(T* from, T* to) noexcept
{
  if constexpr (is_array_v<T>) {
#if __cpp_lib_start_lifetime_as >= 202207L
    std::relocate(std::begin(*from), std::end(*from),
                  std::begin(*std::start_lifetime_as<T>(to)));
#else
    std::relocate(std::begin(*from), std::end(*from), std::begin(*to));
#endif
  }
  else {
    ::new ((void*) to) T(std::move(*from));
    from->~T();
  }
}

template <class T>
requires (is_nothrow_move_constructible_v<T>)
constexpr T* __relocate_by_move_destroy(T* first, T* last, T* result) noexcept
{
  if (__ptr_is_in_range(result, first, last)) {
    result += (last - first);
    auto ret = result;
    while (first != last)
      __move_destroy_element<T>(--last, --result);
    return ret;
  }
  else {
    while (first != last)
      __move_destroy_element<T>(first++, result++);
    return result;
  }
}

template <class T>
constexpr T* relocate(T* first, T* last, T* result) noexcept
{
  static_assert(is_nothrow_relocatable_v<T>);


  if (result == first)
    return result + (last - first);

  if constexpr (! is_trivially_relocatable_v<T>) {
    // No choice but to use move-destroy.
    return __relocate_by_move_destroy(first, last, result);
  }
  else if constexpr (! is_move_constructible_v<T>) {
    // No choice but to use trivial relocation.
    // If got here, then call becomes not `constexpr` evaluable.
    return trivially_relocate(first, last, result);
  }
  else if consteval {
    // Trivially relocatable, but not in `consteval` context.
    return __relocate_by_move_destroy(first, last, result);
  }
  else {
    // Preferred choice when no disqualifications are present.
    return trivially_relocate(first, last, result);
  }
}

} // Close namespace std

#endif // ! defined(INCLUDED_CLANG_RELOCATE)

// Local Variables:
// c-basic-offset: 2
// End:
