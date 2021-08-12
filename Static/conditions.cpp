
#include "pch.h"
#include "framework.h"

#include <conditions.h>
#include <algorithm>

namespace scheduling {

// Any
template <Unit Begin, Unit End>
Any<Begin, End>::Any() : _current(Begin) {}

template <Unit Begin, Unit End>
void Any<Begin, End>::init(const Unit current) {
  // NB: exctra check. We could assume current >= Begin
  _current = std::max(current, Begin);
}

template <Unit Begin, Unit End>
void Any<Begin, End>::reset() {
  _current = Begin;
}

template <Unit Begin, Unit End>
Any<Begin, End>::operator bool() const {
  return _current <= End;
}

template <Unit Begin, Unit End>
Unit Any<Begin, End>::operator*() const {
  return _current;
}

template <Unit Begin, Unit End>
typename Any<Begin, End>::self_t& Any<Begin, End>::operator++() {
  ++_current;
  return *this;
}


// RAny
template <Unit Begin, Unit End>
RAny<Begin, End>::RAny() : _current(End) {}

template <Unit Begin, Unit End>
void RAny<Begin, End>::init(const Unit current) {
  // NB: exctra check. We could assume current >= Begin
  _current = std::min(current, End);
}

template <Unit Begin, Unit End>
void RAny<Begin, End>::reset() {
  _current = End;
}

template <Unit Begin, Unit End>
RAny<Begin, End>::operator bool() const {
  return _current >= Begin;
}

template <Unit Begin, Unit End>
Unit RAny<Begin, End>::operator*() const {
  return _current;
}

template <Unit Begin, Unit End>
typename RAny<Begin, End>::self_t& RAny<Begin, End>::operator++() {
  --_current;
  return *this;
}


// Const
Const::Const(const Unit val) : _val(val), _active(true) {}
void Const::init(const Unit current) { _active = current <= _val; }
void Const::reset() { _active = true; }
Const::operator bool() const { return _active; }
Unit Const::operator*() const { return _val; }
Const& Const::operator++() {
  _active = false;
  return *this;
}


// RConst
RConst::RConst(const Unit val) : _val(val), _active(true) {}
void RConst::init(const Unit current) { _active = current >= _val; }
void RConst::reset() { _active = true; }
RConst::operator bool() const { return _active; }
Unit RConst::operator*() const { return _val; }
RConst& RConst::operator++() {
  _active = false;
  return *this;
}


// Range
Range::Range(const Unit from, const Unit to)
    : _to(to), _from(from), _current(from) {}
void Range::init(const Unit current) { _current = std::max(current, _from); }
void Range::reset() { _current = _from; }
Range::operator bool() const { return _current <= _to; }
Unit Range::operator*() const { return _current; }
Range& Range::operator++() {
  ++_current;
  return *this;
}

// RRange
RRange::RRange(const Unit from, const Unit to)
    : _to(to), _from(from), _current(to) {}
void RRange::init(const Unit current) { _current = std::min(current, _to); }
void RRange::reset() { _current = _to; }
RRange::operator bool() const { return _current >= _from; }
Unit RRange::operator*() const { return _current; }
RRange& RRange::operator++() {
  --_current;
  return *this;
}


// Any step
template <Unit Begin, Unit End>
AnyStep<Begin, End>::AnyStep(Unit step) : _step(step) {}

// todo: copypast with Range
template <Unit Begin, Unit End>
void AnyStep<Begin, End>::init(const Unit current) {
  if (current <= Begin) {
    Any<Begin, End>::_current = Begin;
    return;
  }
  Any<Begin, End>::_current = current;

  auto diffmod = (current - Begin) % _step;
  if (diffmod != 0) {
    Any<Begin, End>::_current += _step - diffmod;
  }
}

template <Unit Begin, Unit End>
AnyStep<Begin, End>& AnyStep<Begin, End>::operator++() {
  Any<Begin, End>::_current += _step;
  return *this;
}

// RAny step
template <Unit Begin, Unit End>
RAnyStep<Begin, End>::RAnyStep(Unit step) : _step(step) {}

template <Unit Begin, Unit End>
void RAnyStep<Begin, End>::init(const Unit current) {
  if (current >= End) {
    RAny<Begin, End>::_current = End;
    RAny<Begin, End>::_current -= (End - Begin) % _step;
    return;
  }
  RAny<Begin, End>::_current = current;
  if (current > Begin) {
    RAny<Begin, End>::_current -= (current - Begin) % _step;
  }
}

template <Unit Begin, Unit End> 
RAnyStep<Begin, End>& RAnyStep<Begin, End>::operator++() {
  RAny<Begin, End>::_current -= _step;
  return *this;
}

template <Unit Begin, Unit End>
void RAnyStep<Begin, End>::reset() {
  RAny<Begin, End>::_current = End;
  RAny<Begin, End>::_current -= (End - Begin) % _step;
}


// RangeStep
RangeStep::RangeStep(const Unit step, const Unit from, const Unit to)
    : Range(from, to), _step(step) {}

void RangeStep::init(const Unit current) {
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

RangeStep& RangeStep::operator++() {
  _current += _step;
  return *this;
}

// RRangeStep
RRangeStep::RRangeStep(const Unit step, const Unit from, const Unit to)
    : RRange(from, to), _step(step) {}

void RRangeStep::init(const Unit current) {
  if (current >= _to) {
    _current = _to;
    _current -= (_to - _from) % _step;
    return;
  }

  _current = current;
  if (_current > _from) {
    _current -= (current - _from) % _step;
  }
}

RRangeStep& RRangeStep::operator++() {
  _current += _step;
  return *this;
}

void RRangeStep::reset() {
  _current = _to;
  _current -= (_to - _from) % _step;
}

template class Any<2000, 2100>;
template class Any<1, 12>;
template class Any<1, 31>;
template class Any<0, 23>;
template class Any<0, 59>;
template class Any<0, 999>;

template class Any<28, 31>;

template class AnyStep<2000, 2100>;
template class AnyStep<1, 12>;
template class AnyStep<1, 31>;
template class AnyStep<0, 23>;
template class AnyStep<0, 59>;
template class AnyStep<0, 999>;


template class RAny<2000, 2100>;
template class RAny<1, 12>;
template class RAny<1, 31>;
template class RAny<0, 23>;
template class RAny<0, 59>;
template class RAny<0, 999>;

template class RAny<28, 31>;

template class RAnyStep<2000, 2100>;
template class RAnyStep<1, 12>;
template class RAnyStep<1, 31>;
template class RAnyStep<0, 23>;
template class RAnyStep<0, 59>;
template class RAnyStep<0, 999>;

}  // namespace scheduling
