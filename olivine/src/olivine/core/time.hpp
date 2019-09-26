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
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/types.hpp"
#include "olivine/core/macros.hpp"

// ========================================================================== //
// Time Declaration
// ========================================================================== //

namespace olivine {

/** \class Time
 * \author Filip Björklund
 * \date 26 september 2019 - 19:44
 * \brief
 * \details
 * Represents time, either as a timestep or as a duration. It should be clear
 * from the situation when it represents either type.
 */
class Time
{
  OL_DEFAULT_COPY(Time);

private:
  /* Internally time is stored in microseconds as an integer. This ensures high
   * resolution */
  u64 mMicroseconds;

public:
  /** Construct a time object from microseconds.
   * \brief Construct time from microseconds.
   * \param microseconds Microseconds to create for.
   */
  Time(u64 microseconds = 0);

  /** Returns the value of the time in microseconds. If the Time object
   * represents a timestamp then this value will only make sense when being used
   * relatively to another value.
   * \brief Returns microseconds value.
   * \return Microseconds.
   */
  u64 GetMicroseconds() const { return mMicroseconds; }

  /** Returns the value of the time in seconds. If the Time object represents a
   * timestamp then this value will only make sense when being used relatively
   * to another value.
   * \brief Returns seconds value.
   * \return Seconds.
   */
  f64 GetSeconds() const { return mMicroseconds / 1000000.0; }

  /* Addition operator */
  friend Time operator+(const Time& lhs, const Time& rhs);

  /* Subtraction operator */
  friend Time operator-(const Time& lhs, const Time& rhs);

  /* Multiplication operator */
  friend Time operator*(const Time& lhs, u64 value);

  /* Increment operator */
  friend void operator+=(Time& lhs, const Time& rhs);

  /* Decrement operator */
  friend void operator-=(Time& lhs, const Time& rhs);

  /* Greater-than operator */
  friend bool operator>(const Time& lhs, const Time& rhs);

  /* Less-than operator */
  friend bool operator<(const Time& lhs, const Time& rhs);

  /* Greater-than or equal-to operator */
  friend bool operator>=(const Time& lhs, const Time& rhs);

  /* Less-than or equal-to operator */
  friend bool operator<=(const Time& lhs, const Time& rhs);

  /* Equality operator */
  friend bool operator==(const Time& lhs, const Time& rhs);

  /* Inequality operator */
  friend bool operator!=(const Time& lhs, const Time& rhs);

public:
  /** Returns a Time object representing the current timestamp.
   * \brief Returns current timestamp.
   * \return Current timestamp.
   */
  static Time Now();

  /** Returns Time object representing a duration from the specified number of
   * seconds.
   * \brief Create duration in seconds.
   * \return Seconds duration.
   */
  static Time FromSeconds(f64 seconds);

  /** Busy-wait for the specified duration of time.
   * \note This will have the processor running a tight while loop until the
   * duration of time has passed, and can therefore not be considered use where
   * sleep should instead be used.
   * \brief Busy-wait.
   * \param duration Duration of time to wait for.
   */
  static void BusyWait(Time duration);
};

}