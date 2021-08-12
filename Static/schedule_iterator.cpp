#include "pch.h"
#include "framework.h"

#include <schedule_iterator.h>

#include <tuple>

#include <boost/date_time/gregorian/gregorian_types.hpp>

namespace scheduling {

  bool operator==(const Time& a, const Time& b) {
  return std::tie(a.milliseconds, a.seconds, a.minutes, a.hours) ==
         std::tie(b.milliseconds, b.seconds, b.minutes, b.hours);
}

bool operator==(const Date& a, const Date& b) {
  return std::tie(a.days, a.months, a.years) ==
         std::tie(b.days, b.months, b.years);
}

bool operator==(const DateTime& a, const DateTime& b) {
  return a.date == b.date && a.time == b.time;
}


void Iter::init(const DateTime& current) {
  // NB: suppose to use std::chrono::year_month_day
  //     and std::chrono::hh_mm_ss
  // But VS does not support C++20 yet

  yearsSequence.init(current.date.years);
  monthsSequence.init(current.date.months);
  daysSequence.init(current.date.days);

  hoursSequence.init(current.time.hours);
  minutesSequence.init(current.time.minutes);
  secondsSequence.init(current.time.seconds);
  millisecondsSequence.init(current.time.milliseconds);
}

Iter::operator bool() const { return yearsSequence; }

DateTime Iter::operator*() const {
  return DateTime{Date{*daysSequence, *monthsSequence, *yearsSequence},
                  Time{
                      *millisecondsSequence,
                      *secondsSequence,
                      *minutesSequence,
                      *hoursSequence,
                  }};
}


Iter& Iter::operator++() {
  // If found next time at the same date - done
  if (increment_time()) {
    return *this;
  }

  increment_date();

  // Increment date until the end of iterator or found first valid date
  while (bool(*this) && !is_valid_date()) {
    increment_date();
  }

  return *this;
}

bool Iter::increment_sector(const std::vector<Combiner*>& sector) {
  for (auto* s : sector) {
    ++(*s);
    // if sequence have next value: found next valid value
    if (*s) {
      return true;
    }
    s->reset();
  }

  // Next value was not found: reset all time sequence
  return false;
}

bool Iter::is_valid_date() {
  return boost::gregorian::gregorian_calendar::end_of_month_day(
             *yearsSequence, *monthsSequence) >= *daysSequence;
}

bool Iter::increment_time() {
  return increment_sector(std::vector<Combiner*>{
      &millisecondsSequence,
      &secondsSequence,
      &minutesSequence,
      &hoursSequence,
  });
}

bool Iter::increment_date() {
  return increment_sector(std::vector<Combiner*>{
      &daysSequence,
      &monthsSequence,
      &yearsSequence,
  });
}


}  // namespace scheduling
