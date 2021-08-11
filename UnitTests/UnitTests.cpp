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
	public:
		
		TEST_METHOD(TestYears)
		{
			Units units;
			scheduling::AnyYear ay;

			ay.init(2020);
      while (ay) {
        units.push_back(*ay);
        ++ay;
			}

			Units expected;
      for (int i = 2020; i <= 2100; ++i) {
        expected.push_back(i);
			}

			Assert::IsTrue(units == expected);
		}  // namespace UnitTests

    TEST_METHOD(TestStepDays) {
      Units units;
      scheduling::AnyStepDay ay(3);

      ay.init(14);

      while (ay) {
        units.push_back(*ay);
        ++ay;
      }

      Units expected{16, 19, 22, 25, 28, 31};
      Assert::IsTrue(units == expected);
    }

    TEST_METHOD(TestRangeStep) {
      Units units;
      scheduling::RangeStep ay(5, 1, 18);

      ay.init(3);

      while (ay) {
        units.push_back(*ay);
        ++ay;
      }

      Units expected{6, 11, 16};
      Assert::IsTrue(units == expected);
    }
	};
}
