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

#include "olivine/core/time.hpp"

// ========================================================================== //
// Headers
// ========================================================================== //

// Project headers
#include "olivine/core/platform/headers.hpp"

// ========================================================================== //
// Time Implementation
// ========================================================================== //

namespace olivine {

Time::Time(u64 microseconds)
  : mMicroseconds(microseconds)
{}

// -------------------------------------------------------------------------- //

Time
operator+(const Time& lhs, const Time& rhs)
{
  return Time{ lhs.mMicroseconds + rhs.mMicroseconds };
}

// -------------------------------------------------------------------------- //

Time
operator-(const Time& lhs, const Time& rhs)
{
  return Time{ lhs.mMicroseconds - rhs.mMicroseconds };
}

// -------------------------------------------------------------------------- //

Time operator*(const Time& lhs, u64 value)
{
  return lhs.mMicroseconds * value;
}

// -------------------------------------------------------------------------- //

void
operator+=(Time& lhs, const Time& rhs)
{
  lhs.mMicroseconds += rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

void
operator-=(Time& lhs, const Time& rhs)
{
  lhs.mMicroseconds -= rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

bool
operator>(const Time& lhs, const Time& rhs)
{
  return lhs.mMicroseconds > rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

bool
operator<(const Time& lhs, const Time& rhs)
{
  return lhs.mMicroseconds < rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

bool
operator>=(const Time& lhs, const Time& rhs)
{
  return lhs.mMicroseconds >= rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

bool
operator<=(const Time& lhs, const Time& rhs)
{
  return lhs.mMicroseconds <= rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

bool
operator==(const Time& lhs, const Time& rhs)
{
  return lhs.mMicroseconds == rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

bool
operator!=(const Time& lhs, const Time& rhs)
{
  return lhs.mMicroseconds != rhs.mMicroseconds;
}

// -------------------------------------------------------------------------- //

Time
Time::Now()
{
  // Frequency
  static u64 frequency = 0;
  if (frequency == 0) {
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    frequency = f.QuadPart;
  }

  // Startup counter
  // TODO(Filip Björklund): See why this all of a sudden started to be a
  //  problem. It starts off high enough that we get rounding errors when
  //  converting to seconds.
  static u64 start_counter = 0;
  if (start_counter == 0) {
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    start_counter = c.QuadPart;
  }

  // Query counter
  LARGE_INTEGER c;
  QueryPerformanceCounter(&c);
  const u64 counter = c.QuadPart - start_counter;

  return Time{ counter * 1000000 / frequency };
}

// -------------------------------------------------------------------------- //

Time
Time::FromSeconds(f64 seconds)
{
  return Time{ u64(seconds * 1000000.0) };
}

// -------------------------------------------------------------------------- //

void
Time::BusyWait(Time duration)
{
  const Time start = Now();
  while (Now() - start < duration) {
  }
}

}
