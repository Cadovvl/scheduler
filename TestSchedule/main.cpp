#include <iostream>

using Unit = int16_t;



class ICondition {
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
  virtual ICondition& operator++() = 0;
};

template<Unit Begin, Unit End>
class Any : public ICondition {
  typedef Any<Begin, End> self_t;
protected:
  Unit _current;
public:
  Any() : _current(Begin) {}

  void init(const Unit current) override final {
    _current = current;
  }

  void reset() override final {
    _current = Begin;
  }

  virtual operator bool() const override final {
    return _current <= End;
  }

  Unit operator*() const override final {
    return _current;
  }

  self_t& operator++() override {
    ++_current;
    return *this;
  }
};

class Const : public ICondition {
  typedef Const self_t;
  const Unit _val;
  bool _active;
public:
  explicit Const(const Unit val) : _val(val), _active(true) {}

  void init(const Unit current) override final {
    _active = current <= _val;
  }

  void reset() override final {
    _active = true;
  }

  virtual operator bool() const override final {
    return _active;
  }

  Unit operator*() const override final {
    return _val;
  }

  self_t& operator++() override final {
    _active = false;
    return *this;
  }
};

class Range : public ICondition {
  const Unit _to;
protected:
  const Unit _from;
protected:
  Unit _current;
public:

  Range(const Unit from, const  Unit to)
    : _to(to), _from(from), _current(from) {}

  virtual void init(const Unit current) override {
    _current = current;
  }

  virtual void reset() override final {
    _current = _from;
  }

  // returns false if there is no next value
  virtual operator bool() const override final {
    return _current <= _to;
  }

  // returns next available value
  virtual Unit operator*() const override final {
    return _current;
  }

  // iterates to the next iterator
  virtual Range& operator++() override {
    ++_current;
    return *this;
  }

};

template<Unit Begin, Unit End>
class AnyStep final : public Any<Begin, End> {
  const Unit _step;
public:
  explicit AnyStep(Unit step) : _step(step) {}

  virtual AnyStep& operator++() override final {
    Any<Begin, End>::_current += _step;
    return *this;
  }
};


class RangeStep final : public Range {
  const Unit _step;
public:
  explicit RangeStep(const Unit step, const Unit from, const Unit to) 
    : Range(from, to), _step(step) {}

  virtual void init(const Unit current) override {
    if (current <= _from) {
      _current = _from;
      return;
    }
    _current = current;
    auto diffmod = (_current - _from) % _step;
    if (diffmod != 0) {
      _current += _step - diffmod;
    }
  }

  virtual RangeStep& operator++() override final {
    _current += _step;
    return *this;
  }
};



using AnyHour = Any<0, 23>;
using AnyDay = Any<0, 31>;

using AnyStepHour = AnyStep<0, 23>;
using AnyStepDay = AnyStep<0, 31>;

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


  std::cout << "HW" << std::endl;

  return 0;
}
