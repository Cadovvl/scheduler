#include "pch.h"
#include "CppUnitTest.h"

#include <vector>

#include <conditions.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
typedef std::vector<scheduling::Unit> Units;

	TEST_CLASS(TestConditions)
	{
    static void FillResults(Units& units, scheduling::ICondition& condition) {
      units.clear();
      while (condition) {
        units.push_back(*condition);
        ++condition;
      }
    }
	public:
		
		TEST_METHOD(TestYears)
		{
			Units units;
			scheduling::AnyYear condition;

			condition.init(2020);
      FillResults(units, condition);

			Units expected;
      for (int i = 2020; i <= 2100; ++i) {
        expected.push_back(i);
			}

			Assert::IsTrue(units == expected);
		}  // namespace UnitTests

    TEST_METHOD(TestStepDays) {
      Units units;
      scheduling::AnyStepDay condition(3);

      condition.init(14);
      FillResults(units, condition);

      Units expected{16, 19, 22, 25, 28, 31};
      Assert::IsTrue(units == expected);
    }

    TEST_METHOD(TestRangeStep) {
      Units units;
      scheduling::RangeStep condition(5, 1, 18);

      condition.init(3);
      FillResults(units, condition);

      Units expected{6, 11, 16};
      Assert::IsTrue(units == expected);
    }

    TEST_METHOD(TestRangeSmallInit) {
      Units units;
      scheduling::Range condition(14, 18);

      condition.init(3);
      FillResults(units, condition);

      Units expected{14, 15, 16, 17, 18};
      Assert::IsTrue(units == expected);
    }
	};
}
