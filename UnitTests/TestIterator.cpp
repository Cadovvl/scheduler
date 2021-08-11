#include "pch.h"
#include "CppUnitTest.h"


#include <vector>
#include <schedule_iterator.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests {
typedef std::vector<scheduling::Unit> Units;

TEST_CLASS(TestIterator) {
  scheduling::Iter::TimePoint date_to_tp(
      scheduling::Unit ms, scheduling::Unit sec, scheduling::Unit min,
      scheduling::Unit hour, scheduling::Unit mday, scheduling::Unit mon,
      scheduling::Unit year){

    
    std::tm tm = {
        /* .tm_sec  = */ sec,
        /* .tm_min  = */ min,
        /* .tm_hour = */ hour,
        /* .tm_mday = */ mday,
        /* .tm_mon  = */ mon - 1,
        /* .tm_year = */ (year) - 1900,
    };
    
    auto tt = _mkgmtime(&tm);

    return std::chrono::system_clock::from_time_t(tt) +
       std::chrono::milliseconds{ms};
  }


 public:
  TEST_METHOD(TestSimpleIterator) {

    // "*.9.*/2 10:00:00.000"
    scheduling::Iter iter;
    iter.yearsSequence.emplace(std::make_unique<scheduling::AnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::Const>(9));
    iter.daysSequence.emplace(std::make_unique<scheduling::AnyStepDay>(2));


    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(10));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(0));

    auto date = date_to_tp(
        /*ms*/ 0,
        /*sec*/ 0,
        /*min*/ 0,
        /*hour*/ 0,
        /*mday*/ 28,
        /*mon*/ 2,
        /*year*/ 2010);

    auto n1 = date_to_tp(
        /*ms*/ 0,
        /*sec*/ 0,
        /*min*/ 0,
        /*hour*/ 10,
        /*mday*/ 29,
        /*mon*/ 9,
        /*year*/ 2010);

    auto n2 = date_to_tp(
        /*ms*/ 0,
        /*sec*/ 0,
        /*min*/ 0,
        /*hour*/ 10,
        /*mday*/ 31,
        /*mon*/ 9,
        /*year*/ 2010);

    auto n3 = date_to_tp(
        /*ms*/ 0,
        /*sec*/ 0,
        /*min*/ 0,
        /*hour*/ 10,
        /*mday*/ 1,
        /*mon*/ 9,
        /*year*/ 2011);


    iter.init(date);


    Assert::IsTrue(*iter == n1);
    ++iter;
    Assert::IsTrue(*iter == n2);
    ++iter;
    Assert::IsTrue(*iter == n3);

  }
 };
}


