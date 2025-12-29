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
constexpr inline bool
is_trivially_relocatable_v = __builtin_is_cpp_trivially_relocatable(T);

template <class T>
struct is_trivially_relocatable :
    bool_constant<is_trivially_relocatable_v<T>> {};

template <class T>
constexpr inline bool is_replaceable_v = __builtin_is_replaceable(T);

template <class T>
struct is_replaceable : bool_constant<is_replaceable_v<T>> {};

template <class T>
constexpr inline bool
is_nothrow_relocatable_v =
  is_trivially_relocatable_v<T> || is_nothrow_move_constructible_v<T>;

template <class T>
struct is_nothrow_relocatable :
    bool_constant<is_nothrow_relocatable_v<T>> {};


// `std::trivially_relocate()`

template <class T>
requires (is_trivially_relocatable_v<T>)
inline T* trivially_relocate(T* first, T* last, T* result) {
  return __builtin_trivially_relocate(result, first, last-first);
}

// `std::relocate()`

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
requires (is_nothrow_move_constructible_v<T>)
constexpr T* __relocate_by_move_destroy(T* first, T* last, T* result) noexcept
{
  if (__ptr_is_in_range(result, first, last)) {
    result += (last - first);
    auto ret = result;
    while (first != last) {
      ::new ((void*) --result) T(std::move(*--last));
      last->~T();
    }
    return ret;
  }
  else {
    while (first != last) {
      ::new ((void*) result++) T(std::move(*first));
      (first++)->~T();
    }
    return result;
  }
}

template <class T>
requires (is_nothrow_relocatable_v<T>)
constexpr T* relocate(T* first, T* last, T* result) noexcept
{
  if (result == first)
    return result + (last - first);

  if constexpr (! is_trivially_relocatable_v<T>) {
    return __relocate_by_move_destroy(first, last, result);
  }
  else if constexpr (! is_nothrow_move_constructible_v<T>) {
    // If got here, then call is not `constexpr` evaluable.
    return trivially_relocate(first, last, result);
  }
  else if consteval {
    return __relocate_by_move_destroy(first, last, result);
  }
  else {
    return trivially_relocate(first, last, result);
  }
}
} // Close namespace std

#endif // ! defined(INCLUDED_CLANG_RELOCATE)

// Local Variables:
// c-basic-offset: 2
// End:
