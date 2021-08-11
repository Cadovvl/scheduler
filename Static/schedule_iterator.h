#pragma once

#include <chrono>

#include <combiner.h>

namespace scheduling {

class Iter {
 public:
  typedef std::chrono::system_clock::time_point TimePoint;
  typedef std::chrono::system_clock::duration Duration;

  Combiner yearsSequence;
  Combiner monthsSequence;
  Combiner daysSequence;

  Combiner hoursSequence;
  Combiner minutesSequence;
  Combiner secondsSequence;
  Combiner millisecondsSequence;

  // init with current date
  void init(const TimePoint& current);
  // returns false if there is no next value
  operator bool() const;
  // returns next available value
  TimePoint operator*() const;
  // iterates to the next time point
  Iter& operator++();
};

}  // namespace scheduling
