#pragma once
#include <cstdint>

namespace scheduling {

using Unit = int16_t;

class ISequence {
 public:
  // Setup condition with current value
  virtual void init(const Unit current) = 0;
  // Reset condition to the beginning
  virtual void reset() = 0;
  // returns false if there is no next value
  virtual operator bool() const = 0;
  // returns next available value
  virtual Unit operator*() const = 0;
  // iterates to the next iterator
  virtual ISequence& operator++() = 0;
};

template <Unit Begin, Unit End>
class Any : public ISequence {
  typedef Any<Begin, End> self_t;

 protected:
  Unit _current;

 public:
  Any();

  /*overrides*/
  void init(const Unit current) override;
  void reset() override final;
  operator bool() const override final;
  Unit operator*() const override final;
  self_t& operator++() override;
};


template <Unit Begin, Unit End>
class RAny : public ISequence {
  typedef RAny<Begin, End> self_t;

 protected:
  Unit _current;

 public:
  RAny();

  /*overrides*/
  void init(const Unit current) override;
  void reset() override;
  operator bool() const override final;
  Unit operator*() const override final;
  self_t& operator++() override;
};


class Const final : public ISequence {
  const Unit _val;
  bool _active;

 public:
  explicit Const(const Unit val);

  /*overrides*/
  void init(const Unit current) override final;
  void reset() override final;
  operator bool() const override final;
  Unit operator*() const override final;
  Const& operator++() override final;
};


class RConst final : public ISequence {
  const Unit _val;
  bool _active;

 public:
  explicit RConst(const Unit val);

  /*overrides*/
  void init(const Unit current) override final;
  void reset() override final;
  operator bool() const override final;
  Unit operator*() const override final;
  RConst& operator++() override final;
};


class Range : public ISequence {
  const Unit _to;

 protected:
  const Unit _from;

 protected:
  Unit _current;

 public:
  Range(const Unit from, const Unit to);

  /*overrides*/
  void init(const Unit current) override;
  void reset() override final;
  operator bool() const override final;
  Unit operator*() const override final;
  Range& operator++() override;
};


class RRange : public ISequence {
 protected:
  const Unit _from;
  const Unit _to;

 protected:
  Unit _current;

 public:
  RRange(const Unit from, const Unit to);

  /*overrides*/
  void init(const Unit current) override;
  void reset() override;
  operator bool() const override final;
  Unit operator*() const override final;
  RRange& operator++() override;
};


template <Unit Begin, Unit End>
class AnyStep final : public Any<Begin, End> {
  const Unit _step;

 public:
  explicit AnyStep(Unit step);

  /*overrides*/
  void init(const Unit current) override final;
  AnyStep& operator++() override final;
};


template <Unit Begin, Unit End>
class RAnyStep final : public RAny<Begin, End> {
  const Unit _step;

 public:
  explicit RAnyStep(Unit step);

  /*overrides*/
  void init(const Unit current) override final;
  void reset() override final;
  RAnyStep& operator++() override final;
};

class RangeStep final : public Range {
  const Unit _step;

 public:
  explicit RangeStep(const Unit step, const Unit from, const Unit to);

  /*overrides*/
  void init(const Unit current) override final;
  RangeStep& operator++() override final;
};


class RRangeStep final : public RRange {
  const Unit _step;

 public:
  explicit RRangeStep(const Unit step, const Unit from, const Unit to);

  /*overrides*/
  void init(const Unit current) override final;
  void reset() override final;
  RRangeStep& operator++() override final;
};

using AnyYear = Any<2000, 2100>;
using AnyMonth = Any<1, 12>;
using AnyDay = Any<1, 31>;
using AnyHour = Any<0, 23>;
using AnyMinute = Any<0, 59>;
using AnySecond = Any<0, 59>;
using AnyMillisecond = Any<0, 999>;

using LastDay = Any<28, 31>;

using AnyStepYear = AnyStep<2000, 2100>;
using AnyStepMonth = AnyStep<1, 12>;
using AnyStepDay = AnyStep<1, 31>;
using AnyStepHour = AnyStep<0, 23>;
using AnyStepMinute = AnyStep<0, 59>;
using AnyStepSecond = AnyStep<0, 59>;
using AnyStepMillisecond = AnyStep<0, 999>;

using RAnyYear = RAny<2000, 2100>;
using RAnyMonth = RAny<1, 12>;
using RAnyDay = RAny<1, 31>;
using RAnyHour = RAny<0, 23>;
using RAnyMinute = RAny<0, 59>;
using RAnySecond = RAny<0, 59>;
using RAnyMillisecond = RAny<0, 999>;

using RLastDay = RAny<28, 31>;

using RAnyStepYear = RAnyStep<2000, 2100>;
using RAnyStepMonth = RAnyStep<1, 12>;
using RAnyStepDay = RAnyStep<1, 31>;
using RAnyStepHour = RAnyStep<0, 23>;
using RAnyStepMinute = RAnyStep<0, 59>;
using RAnyStepSecond = RAnyStep<0, 59>;
using RAnyStepMillisecond = RAnyStep<0, 999>;


}  // namespace scheduling

