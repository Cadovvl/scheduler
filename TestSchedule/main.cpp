#include <iostream>

#include <schedule_iterator.h>
#include <chrono>
#include <algorithm>
using namespace scheduling;

constexpr size_t N = 10000;
constexpr size_t K = 1;


template <typename IterType>
void check_iter(IterType& iter, const scheduling::DateTime& dt) {
  std::vector<size_t> ns;

  ns.reserve(N);

  using namespace std::chrono;

  for (size_t i = 0; i < N; ++i) {

    
    size_t counter = 0;
    for (size_t j = 0; j < K; ++j) {
      std::chrono::steady_clock::time_point begin =
          std::chrono::steady_clock::now();

      iter.init(dt);

      std::chrono::steady_clock::time_point end =
          std::chrono::steady_clock::now();
      counter += duration_cast<nanoseconds>(end - begin).count();
    }

    ns.push_back(counter / K);
  }

  std::sort(ns.begin(), ns.end());

  std::cout << " P90: " << ns[90 * N / 100]
            << " ns; P95: " << ns[95 * N / 100]
            << " ns; P98: " << ns[98 * N / 100] << " ns;" << std::endl;
}

int main() {

  std::cout << "N: " << N << "\tK:" << K << std::endl;

  {
    // *.*.1 0:0:0
    scheduling::Iter iter;

    iter.yearsSequence.emplace(std::make_unique<scheduling::AnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::AnyMonth>());
    iter.daysSequence.emplace(std::make_unique<scheduling::Const>(1));

    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(0));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(0));

    check_iter(iter, scheduling::DateTime{scheduling::Date{01, 01, 2001},
                                          scheduling::Time{0, 0, 0, 0}});
    check_iter(iter, scheduling::DateTime{scheduling::Date{05, 05, 2080},
                                          scheduling::Time{0, 0, 0, 0}});
  }
  std::cout << std::endl;

  {
    // *.4.6,7 * *:*:*.1,2,3-5,10-20/3
    scheduling::Iter iter;

    iter.yearsSequence.emplace(std::make_unique<scheduling::AnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::Const>(4));
    iter.daysSequence.emplace(std::make_unique<scheduling::Const>(6));
    iter.daysSequence.emplace(std::make_unique<scheduling::Const>(7));

    iter.hoursSequence.emplace(std::make_unique<scheduling::AnyHour>());
    iter.minutesSequence.emplace(std::make_unique<scheduling::AnyMinute>());
    iter.secondsSequence.emplace(std::make_unique<scheduling::AnySecond>());

    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(1));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(2));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Range>(3,5));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::RangeStep>(3, 10, 20));

    check_iter(iter, scheduling::DateTime{scheduling::Date{01, 01, 2001},
                                          scheduling::Time{0, 0, 0, 0}});
    check_iter(iter, scheduling::DateTime{scheduling::Date{05, 05, 2080},
                                          scheduling::Time{0, 0, 0, 0}});
  }

  std::cout << std::endl;
  {
    // 2100.12.31 23:59:59.999
    scheduling::Iter iter;

    iter.yearsSequence.emplace(std::make_unique<scheduling::Const>(2100));
    iter.monthsSequence.emplace(std::make_unique<scheduling::Const>(12));
    iter.daysSequence.emplace(std::make_unique<scheduling::Const>(31));

    iter.hoursSequence.emplace(std::make_unique<scheduling::Const>(23));
    iter.minutesSequence.emplace(std::make_unique<scheduling::Const>(59));
    iter.secondsSequence.emplace(std::make_unique<scheduling::Const>(59));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::Const>(999));

    check_iter(iter, scheduling::DateTime{scheduling::Date{01, 01, 2001},
                                          scheduling::Time{0, 0, 0, 0}});
    check_iter(iter, scheduling::DateTime{scheduling::Date{05, 05, 2080},
                                          scheduling::Time{0, 0, 0, 0}});
  }

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;

  {
    // *.*.1 0:0:0
    scheduling::RIter iter;

    iter.yearsSequence.emplace(std::make_unique<scheduling::RAnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::RAnyMonth>());
    iter.daysSequence.emplace(std::make_unique<scheduling::RConst>(1));

    iter.hoursSequence.emplace(std::make_unique<scheduling::RConst>(0));
    iter.minutesSequence.emplace(std::make_unique<scheduling::RConst>(0));
    iter.secondsSequence.emplace(std::make_unique<scheduling::RConst>(0));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::RConst>(0));

    check_iter(iter, scheduling::DateTime{scheduling::Date{01, 01, 2001},
                                          scheduling::Time{0, 0, 0, 0}});
    check_iter(iter, scheduling::DateTime{scheduling::Date{05, 05, 2080},
                                          scheduling::Time{0, 0, 0, 0}});
  }
  std::cout << std::endl;

  {
    // *.4.6,7 * *:*:*.1,2,3-5,10-20/3
    scheduling::RIter iter;

    iter.yearsSequence.emplace(std::make_unique<scheduling::RAnyYear>());
    iter.monthsSequence.emplace(std::make_unique<scheduling::RConst>(4));
    iter.daysSequence.emplace(std::make_unique<scheduling::RConst>(6));
    iter.daysSequence.emplace(std::make_unique<scheduling::RConst>(7));

    iter.hoursSequence.emplace(std::make_unique<scheduling::RAnyHour>());
    iter.minutesSequence.emplace(std::make_unique<scheduling::RAnyMinute>());
    iter.secondsSequence.emplace(std::make_unique<scheduling::RAnySecond>());

    iter.millisecondsSequence.emplace(std::make_unique<scheduling::RConst>(1));
    iter.millisecondsSequence.emplace(std::make_unique<scheduling::RConst>(2));
    iter.millisecondsSequence.emplace(
        std::make_unique<scheduling::RRange>(3, 5));
    iter.millisecondsSequence.emplace(
        std::make_unique<scheduling::RRangeStep>(3, 10, 20));

    check_iter(iter, scheduling::DateTime{scheduling::Date{01, 01, 2001},
                                          scheduling::Time{0, 0, 0, 0}});
    check_iter(iter, scheduling::DateTime{scheduling::Date{05, 05, 2080},
                                          scheduling::Time{0, 0, 0, 0}});
  }

  std::cout << std::endl;
  {
    // 2100.12.31 23:59:59.999
    scheduling::Iter iter;

    iter.yearsSequence.emplace(std::make_unique<scheduling::RConst>(2100));
    iter.monthsSequence.emplace(std::make_unique<scheduling::RConst>(12));
    iter.daysSequence.emplace(std::make_unique<scheduling::RConst>(31));

    iter.hoursSequence.emplace(std::make_unique<scheduling::RConst>(23));
    iter.minutesSequence.emplace(std::make_unique<scheduling::RConst>(59));
    iter.secondsSequence.emplace(std::make_unique<scheduling::RConst>(59));
    iter.millisecondsSequence.emplace(
        std::make_unique<scheduling::RConst>(999));

    check_iter(iter, scheduling::DateTime{scheduling::Date{01, 01, 2001},
                                          scheduling::Time{0, 0, 0, 0}});
    check_iter(iter, scheduling::DateTime{scheduling::Date{05, 05, 2080},
                                          scheduling::Time{0, 0, 0, 0}});
  }


  return 0;
}
