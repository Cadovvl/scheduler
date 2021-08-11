#include "pch.h"
#include "framework.h"

#include <schedule_iterator.h>


namespace scheduling {

void Iter::init(const TimePoint& current) {
  // NB: suppose to use std::chrono::year_month_day
  //     and std::chrono::hh_mm_ss
  // But VS does not support C++20 yet

  time_t tt = std::chrono::system_clock::to_time_t(current);
  
  std::tm tm;
  gmtime_s(&tm, &tt);

  yearsSequence.init(tm.tm_year + 1900);
  monthsSequence.init(tm.tm_mon + 1);
  daysSequence.init(tm.tm_mday);

  hoursSequence.init(tm.tm_hour);
  minutesSequence.init(tm.tm_min);

  // seconds includes leap second. Pretending it does no exist
  secondsSequence.init(tm.tm_sec % 60);

  // Bueeeee... Ugly
  millisecondsSequence.init((Unit)std::chrono::duration_cast<std::chrono::milliseconds>(
          current.time_since_epoch() - std::chrono::seconds{tt}).count());
}

Iter::operator bool() const { return *yearsSequence; }

Iter::TimePoint Iter::operator*() const { 
  
  std::tm tm = {
      /* .tm_sec  = */ *secondsSequence,
      /* .tm_min  = */ *minutesSequence,
      /* .tm_hour = */ *hoursSequence,
      /* .tm_mday = */ *daysSequence,
      /* .tm_mon  = */ *monthsSequence - 1,
      /* .tm_year = */ *yearsSequence - 1900,
  };
  
  return std::chrono::system_clock::from_time_t(_mkgmtime(&tm)) +
         std::chrono::milliseconds{*millisecondsSequence};
}


Iter& Iter::operator++() {
  for (auto* s : std::vector<Combiner*>{
           &millisecondsSequence,
           &secondsSequence,
           &minutesSequence,
           &hoursSequence,
           &daysSequence,
           &monthsSequence,
           &yearsSequence,
    }) {
    ++(*s);
    // if sequence have next value -found
    if (*s) {
      break;
    }

    s->reset();
  }

  return *this;
}

}  // namespace scheduling
