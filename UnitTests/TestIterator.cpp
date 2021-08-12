#include "pch.h"
#include "CppUnitTest.h"


#include <vector>
#include <schedule_iterator.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests {
typedef std::vector<scheduling::Unit> Units;

TEST_CLASS(TestIterator) {

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


    iter.init(scheduling::DateTime{scheduling::Date{28, 2, 2010},
                                   scheduling::Time{0, 0, 0, 0}});


    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{1, 9, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{3, 9, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{5, 9, 2010},
                                        scheduling::Time{0, 0, 0, 10}});

    
    iter.init(scheduling::DateTime{scheduling::Date{28, 9, 2010},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 9, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{1, 9, 2011},
                                        scheduling::Time{0, 0, 0, 10}});
  }

  TEST_METHOD(TestInitIterator) {
    // "*.9.*/2 10:00:00.000"
    scheduling::Iter iter;
    iter.yearsSequence.emplace(std::make_unique<scheduling::AnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::Const>(9));
    iter.daysSequence.emplace(std::make_unique<scheduling::AnyStepDay>(2));

    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(10));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(0));

    iter.init(scheduling::DateTime{scheduling::Date{28, 10, 2010},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{1, 9, 2011},
                                        scheduling::Time{0, 0, 0, 10}});

  }

  
  TEST_METHOD(TestInitIterator2) {
    // "2010.5.5 10:01:01.001"
    scheduling::Iter iter;
    iter.yearsSequence.emplace(std::make_unique<scheduling::Const>(2010));
    iter.monthsSequence.emplace(std::make_unique<scheduling::Const>(5));
    iter.daysSequence.emplace(std::make_unique<scheduling::Const>(5));

    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(10));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(1));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(1));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(1));

    iter.init(scheduling::DateTime{scheduling::Date{6, 6, 2011},
                                   scheduling::Time{11, 2, 2, 2}});

    Assert::IsFalse(iter);

    iter.init(scheduling::DateTime{scheduling::Date{6, 6, 2009},
                                   scheduling::Time{11, 2, 2, 2}});

    
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{5, 5, 2010},
                                        scheduling::Time{1, 1, 1, 10}});
    ++iter;
    Assert::IsFalse(iter);
  }

  
  TEST_METHOD(TestMonthsLimit) {
    // "*.*.28-31 10:00:00.000"
    scheduling::Iter iter;
    iter.yearsSequence.emplace(std::make_unique<scheduling::AnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::AnyMonth>());
    iter.daysSequence.emplace(std::make_unique<scheduling::Range>(28, 31));

    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(10));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(0));

    // 2013
    iter.init(scheduling::DateTime{scheduling::Date{1, 1, 2013},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 1, 2013},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 1, 2013},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 1, 2013},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 1, 2013},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 2, 2013},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 3, 2013},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    // 2000
    iter.init(scheduling::DateTime{scheduling::Date{1, 1, 2000},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 1, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 1, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 1, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 1, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 2, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    // NB: 2000 % 1000 == 0
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 2, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 3, 2000},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    // 2004
    iter.init(scheduling::DateTime{scheduling::Date{1, 1, 2004},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 1, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 1, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 1, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 1, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 2, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 2, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 3, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    // 04/2004
    iter.init(scheduling::DateTime{scheduling::Date{1, 4, 2004},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 4, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 4, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 4, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 5, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{29, 5, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 5, 2004},
                                        scheduling::Time{0, 0, 0, 10}});
  }


  TEST_METHOD(TestLastMonthDayIterator) {
    // "*.*.32 10:00:00.000"
    scheduling::Iter iter;
    iter.yearsSequence.emplace(std::make_unique<scheduling::AnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::AnyMonth>());
    iter.daysSequence.emplace(std::make_unique<scheduling::LastDay>());

    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(10));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(0));

    iter.lastMonth = true;

    iter.init(scheduling::DateTime{scheduling::Date{1, 1, 2010},
                                   scheduling::Time{0, 0, 0, 0}});

    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 1, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{28, 2, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 3, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 4, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 5, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{30, 6, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 7, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
    ++iter;
    Assert::IsTrue(*iter ==
                   scheduling::DateTime{scheduling::Date{31, 8, 2010},
                                        scheduling::Time{0, 0, 0, 10}});
  }

 };
}


