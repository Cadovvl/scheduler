#include "pch.h"
#include "CppUnitTest.h"


#include <vector>
#include <combiner.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests {
typedef std::vector<scheduling::Unit> Units;

TEST_CLASS(TestCombiner) {
  static void FillResults(Units& units, scheduling::ICondition& condition) {
    units.clear();
    while (condition) {
      units.push_back(*condition);
      ++condition;
    }
  }
 public:
  TEST_METHOD(TestConstCombiner) {
    Units units;

    // 3,6,15,2,7,28
    scheduling::Combiner combiner;
    combiner.emplace(std::make_unique<scheduling::Const>(3));
    combiner.emplace(std::make_unique<scheduling::Const>(6));
    combiner.emplace(std::make_unique<scheduling::Const>(15));
    combiner.emplace(std::make_unique<scheduling::Const>(2));
    combiner.emplace(std::make_unique<scheduling::Const>(7));
    combiner.emplace(std::make_unique<scheduling::Const>(28));

    combiner.init(6);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{6, 7, 15, 28});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{2, 3, 6, 7, 15, 28});

    combiner.init(26);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{28});

    combiner.init(30);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{2, 3, 6, 7, 15, 28});

  }  

  
  TEST_METHOD(TestConstWithRangeCombiner) {
    Units units;

    // one normal range, one overlaps with const
    // 6,10-15,2-7,8,28
    scheduling::Combiner combiner;
    combiner.emplace(std::make_unique<scheduling::Const>(6));
    combiner.emplace(std::make_unique<scheduling::Range>(10, 15));
    combiner.emplace(std::make_unique<scheduling::Range>(2,7));
    combiner.emplace(std::make_unique<scheduling::Const>(8));
    combiner.emplace(std::make_unique<scheduling::Const>(28));

    combiner.init(13);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{13, 14, 15, 28});

    combiner.init(6);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{6, 7, 8, 10, 11, 12, 13, 14, 15, 28});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units ==
                   Units{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 28});

    combiner.init(0);
    FillResults(units, combiner);
    Assert::IsTrue(units ==
                   Units{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 28});


    combiner.init(30);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});

    combiner.init(28);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{28});

  } 

  TEST_METHOD(TestOverlappingRangesCombiner) {
    Units units;

    // 10-12, 11-14, 9-15
    scheduling::Combiner combiner;
    combiner.emplace(std::make_unique<scheduling::Range>(10, 12));
    combiner.emplace(std::make_unique<scheduling::Range>(11, 14));
    combiner.emplace(std::make_unique<scheduling::Range>(9, 15));

    combiner.init(13);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{13, 14, 15});

    combiner.init(6);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{9, 10, 11, 12, 13, 14, 15});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{9, 10, 11, 12, 13, 14, 15});


    combiner.init(14);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{14, 15});


    combiner.init(16);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});

    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});
  } 

  
  TEST_METHOD(TestComplexCombiner) {
    Units units;

    // */10, 11-14, 16-28/5, 23
    scheduling::Combiner combiner;
    combiner.emplace(std::make_unique<scheduling::AnyStepMinute>(10));
    combiner.emplace(std::make_unique<scheduling::Range>(11, 14));
    combiner.emplace(std::make_unique<scheduling::RangeStep>(5, 16, 28));
    combiner.emplace(std::make_unique<scheduling::Const>(23));

    combiner.init(5);
    FillResults(units, combiner);
    Assert::IsTrue(units ==
                   Units{10, 11, 12, 13, 14, 16, 20, 21, 23, 26, 30, 40, 50});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units ==
                   Units{0, 10, 11, 12, 13, 14, 16, 20, 21, 23, 26, 30, 40, 50});

    combiner.init(13);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{13, 14, 16, 20, 21, 23, 26, 30, 40, 50});

    combiner.init(16);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{16, 20, 21, 23, 26, 30, 40, 50});

    combiner.init(17);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{20, 21, 23, 26, 30, 40, 50});

    combiner.init(27);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{30, 40, 50});
  } 
  

  
  TEST_METHOD(TestEmptyCombiner) {
    Units units;

    scheduling::Combiner combiner;
    combiner.init(5);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});
  } 

  
  
  TEST_METHOD(TestSingleConditionCombiner) {
    Units units;

    scheduling::Combiner combiner;
    combiner.emplace(std::make_unique<scheduling::Const>(10));
    combiner.init(5);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{10});

    combiner.reset();
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{10});

    combiner.init(15);
    FillResults(units, combiner);
    Assert::IsTrue(units == Units{});
  } 

  };

}
