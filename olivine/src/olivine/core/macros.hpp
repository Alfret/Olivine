// MIT License
//
// Copyright (c) 2019 Filip Björklund
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

// ========================================================================== //
// Macros: Functions
// ========================================================================== //

/** Macro for generating operators for an enum class **/
#define OL_ENUM_CLASS_OPERATORS(prefix, type, underlying_type)                 \
  prefix type operator|(type lhs, type rhs)                                    \
  {                                                                            \
    return static_cast<type>(static_cast<underlying_type>(lhs) |               \
                             static_cast<underlying_type>(rhs));               \
  }                                                                            \
  prefix type operator&(type lhs, type rhs)                                    \
  {                                                                            \
    return static_cast<type>(static_cast<underlying_type>(lhs) &               \
                             static_cast<underlying_type>(rhs));               \
  }                                                                            \
  prefix type& operator|=(type& lhs, type rhs)                                 \
  {                                                                            \
    lhs = (lhs | rhs);                                                         \
    return lhs;                                                                \
  }

// -------------------------------------------------------------------------- //

/** Macro for forward declaring a class **/
#define OL_FORWARD_DECLARE(type) class type

// -------------------------------------------------------------------------- //

/** Macro for forward declaring an enum class **/
#define OL_FORWARD_DECLARE_ENUM(type) enum class type

// -------------------------------------------------------------------------- //

/** Macro to mark variable as not being used **/
#define OL_UNUSE(var) (void)var

// -------------------------------------------------------------------------- //

/** Macro to determine if only a single bit is set **/
#define OL_SINGLE_BIT(val) (val && !(val & (val - 1)))

// ========================================================================== //
// Macros: Constants
// ========================================================================== //

/** Macro for declaring a return value as non-discardable **/
#define OL_NODISCARD [[nodiscard]]

// -------------------------------------------------------------------------- //

/** Macro for declaring a function as non-returning **/
#define OL_NORETURN [[noreturn]]

// ========================================================================== //
// Macros: Class Modifiers
// ========================================================================== //

/** Declare class as a namespace class. Only static members **/
#define OL_NAMESPACE_CLASS(cls)                                                \
public:                                                                        \
  cls() = delete;                                                              \
  ~cls() = delete;

// -------------------------------------------------------------------------- //

/* Macro to declare a class as non-copyable */
#define OL_NO_COPY(cls)                                                        \
public:                                                                        \
  cls(const cls&) = delete;                                                    \
  cls& operator=(const cls&) = delete;

// -------------------------------------------------------------------------- //

/* Macro to declare a class as non-movable */
#define OL_NO_MOVE(cls)                                                        \
public:                                                                        \
  cls(cls&&) = delete;                                                         \
  cls& operator=(&cls&) = delete;

// -------------------------------------------------------------------------- //

/* Macro to declare a class as default-copyable */
#define OL_DEFAULT_COPY(cls)                                                   \
public:                                                                        \
  cls(const cls&) = default;                                                   \
  cls& operator=(const cls&) = default;