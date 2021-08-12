#pragma once

#include <chrono>

#include <combiner.h>

namespace scheduling {

struct Time {
  // 0-999
  Unit milliseconds;
  // 0-59
  Unit seconds;
  // 0-59
  Unit minutes;
  // 0-23
  Unit hours;
};

struct Date {
  // 1-31
  Unit days;
  // 1-12
  Unit months;
  // 2000-2100
  Unit years;
};

struct DateTime {
  Date date;
  Time time;
};

bool operator==(const Time& a, const Time& b);
bool operator==(const Date& a, const Date& b);
bool operator==(const DateTime& a, const DateTime& b);

template<bool Reversed>
class IterBase {

  typedef CombinerBase<Reversed> combiner_t;

  bool increment_sector(const std::vector<combiner_t*>& sector);

  bool increment_date();
  bool is_valid_date();
  bool increment_time();

  bool valid_weekday(Unit weekDay);

 public:
  combiner_t yearsSequence;
  combiner_t monthsSequence;
  combiner_t daysSequence;

  combiner_t hoursSequence;
  combiner_t minutesSequence;
  combiner_t secondsSequence;
  combiner_t millisecondsSequence;

  bool lastMonth = false;
  bool filterWeek = false;

  Combiner weekdaySequence;

  // init with current date
  // Sets the first valid date
  void init(const DateTime& current);
  // returns false if there is no next value
  operator bool() const;
  // returns next available value
  DateTime operator*() const;
  // iterates to the next time point
  IterBase<Reversed>& operator++();
};


using Iter = IterBase<false>;
using RIter = IterBase<true>;

}  // namespace scheduling
