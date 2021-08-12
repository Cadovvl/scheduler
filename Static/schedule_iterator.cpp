#include "framework.h"
#include "pch.h"

#include <schedule_iterator.h>

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <tuple>

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

template <bool Reversed>
void IterBase<Reversed>::init(const DateTime& current) {
  // NB: suppose to use std::chrono::year_month_day
  //     and std::chrono::hh_mm_ss
  // But VS does not support C++20 yet

  // reset all
  for (auto* s : std::vector<combiner_t*>{
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

    for (auto [seq, val] : std::vector<std::pair<combiner_t*, Unit>>{
             {&monthsSequence, current.date.months},
             {&daysSequence, current.date.days}}) {
      seq->init(val);
      if (!*seq) goto end_init_block;
      if (**seq != val) goto end_init_block;
    }

    if (!is_valid_date()) goto end_init_block;

    for (auto [seq, val] : std::vector<std::pair<combiner_t*, Unit>>{
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
  for (auto [seq, prev] : std::vector<std::pair<combiner_t*, combiner_t*>>{
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

template <bool Reversed>
IterBase<Reversed>::operator bool() const {
  return yearsSequence;
}

template <bool Reversed>
DateTime IterBase<Reversed>::operator*() const {
  return DateTime{Date{*daysSequence, *monthsSequence, *yearsSequence},
                  Time{
                      *millisecondsSequence,
                      *secondsSequence,
                      *minutesSequence,
                      *hoursSequence,
                  }};
}

template <bool Reversed>
IterBase<Reversed>& IterBase<Reversed>::operator++() {
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

template <bool Reversed>
bool IterBase<Reversed>::increment_sector(
    const std::vector<combiner_t*>& sector) {
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

template <bool Reversed>
bool IterBase<Reversed>::is_valid_date() {
  auto end_of_month_day =
      boost::gregorian::gregorian_calendar::end_of_month_day(*yearsSequence,
                                                             *monthsSequence);
  if (end_of_month_day < *daysSequence) {
    return false;
  }

  if (lastMonth && (end_of_month_day != *daysSequence)) {
    return false;
  }
  boost::gregorian::greg_year_month_day ymd(*yearsSequence, *monthsSequence,
                                            *daysSequence);
  if (filterWeek &&
      !valid_weekday(boost::gregorian::gregorian_calendar::day_of_week(ymd))) {
    return false;
  }

  return true;
}

template <bool Reversed>
bool IterBase<Reversed>::increment_time() {
  return increment_sector(std::vector<combiner_t*>{
      &millisecondsSequence,
      &secondsSequence,
      &minutesSequence,
      &hoursSequence,
  });
}

template <bool Reversed>
bool IterBase<Reversed>::valid_weekday(Unit weekDay) {
  weekdaySequence.reset();
  do {
    if (*weekdaySequence == weekDay) {
      return true;
    }
    ++weekdaySequence;
  } while (weekdaySequence);
  return false;
}

template <bool Reversed>
bool IterBase<Reversed>::increment_date() {
  if (increment_sector( std::vector<combiner_t*>{
    &daysSequence, 
    &monthsSequence})) {
    return true;
  }

  // Year sequence should not be reset;
  ++yearsSequence;
  return true;
}

template class IterBase<true>;
template class IterBase<false>;


}  // namespace scheduling
