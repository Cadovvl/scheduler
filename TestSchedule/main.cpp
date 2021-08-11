#include <iostream>

#include "conditions.h"

using namespace scheduling;

int main() {

  AnyDay day;
  day.init(12);

  while (day) {
    std::cout << "AnyDay: " << *day << std::endl;
    ++day;
  }
  
  Const constDay(12);

  constDay.init(2);

  while (constDay) {
    std::cout << "Const day: " << *constDay << std::endl;
    ++constDay;
  }

  constDay.init(21);

  while (constDay) {
    std::cout << "Const day: " << *constDay << std::endl;
    ++constDay;
  }

  Range r(9, 17);
  r.init(12);
  while (r) {
    std::cout << "Range: " << *r << std::endl;
    ++r;
  }

  AnyStepHour ds(4);
  ds.init(12);
  while (ds) {
    std::cout << "AnyStepHour: " << *ds << std::endl;
    ++ds;
  }

  RangeStep rs(4, 10, 20);
  rs.init(12);
  while (rs) {
    std::cout << "RangeStep: " << *rs << std::endl;
    ++rs;
  }

  rs.reset();
  while (rs) {
    std::cout << "RangeStep reset: " << *rs << std::endl;
    ++rs;
  }


  std::cout << "HW" << std::endl;

  return 0;
}
