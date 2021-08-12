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


  // reset all
  for (auto* s : std::vector<Combiner*>{
           &millisecondsSequence,
           &secondsSequence,
           &minutesSequence,
           &hoursSequence,
           &daysSequence,
           &monthsSequence,
           &yearsSequence,
       }) {
    s->reset();
  }

  // NB: init block 
  {
    yearsSequence.init(current.date.years);
    if (!yearsSequence) goto end_init_block;
    if (*yearsSequence != current.date.years) goto end_init_block;


    for (auto [seq, val] : std::vector<std::pair<Combiner*, Unit>>{
             {&monthsSequence, current.date.months},
             {&daysSequence, current.date.days}}) {
      seq->init(val);
      if (!*seq) goto end_init_block;
      if (**seq != val) goto end_init_block;
    }

    if (!is_valid_date()) goto end_init_block;

    for (auto [seq, val] : std::vector<std::pair<Combiner*, Unit>>{
             {&hoursSequence, current.time.hours},
             {&minutesSequence, current.time.minutes},
             {&secondsSequence, current.time.seconds},
             {&millisecondsSequence, current.time.milliseconds}}) {
      seq->init(val);
      if (!*seq) goto end_init_block;
      if (**seq != val) goto end_init_block;
    }
  }
  end_init_block:


  // Fix combiners without valid value (max one at a time)
  for (auto [seq, prev] : std::vector<std::pair<Combiner*, Combiner*>>{
           {&millisecondsSequence, &secondsSequence},
           {&secondsSequence, &minutesSequence},
           {&minutesSequence, &hoursSequence},
           {&hoursSequence, &daysSequence},
           {&daysSequence, &monthsSequence},
           {&monthsSequence, &yearsSequence},
       }) {
    if (!*seq) {
      seq->reset();
      ++(*prev);
    }
  }
  
  // Increment until valid date
  while (bool(*this) && !is_valid_date()) {
    increment_date();
  }

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
  auto end_of_month_day =
      boost::gregorian::gregorian_calendar::end_of_month_day(*yearsSequence,
                                                             *monthsSequence);
  if (end_of_month_day < *daysSequence) {
    return false;
  }

  if (lastMonth && (end_of_month_day != *daysSequence)) {
    return false;
  }

  return true;
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
  if (increment_sector(std::vector<Combiner*>{
      &daysSequence,
      &monthsSequence})) {
    return true;
  }

  // Year sequence should not be reset;
  ++yearsSequence;
  return true;
}


}  // namespace scheduling
