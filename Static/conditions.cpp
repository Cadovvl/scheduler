
#include "pch.h"
#include "framework.h"

#include <conditions.h>

namespace scheduling {

// Any
template <Unit Begin, Unit End>
Any<Begin, End>::Any() : _current(Begin) {}

template <Unit Begin, Unit End>
void Any<Begin, End>::init(const Unit current) {
  _current = current;
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

// Range
Range::Range(const Unit from, const Unit to)
    : _to(to), _from(from), _current(from) {}
void Range::init(const Unit current) { _current = current; }
void Range::reset() { _current = _from; }
Range::operator bool() const { return _current <= _to; }
Unit Range::operator*() const { return _current; }
Range& Range::operator++() {
  ++_current;
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

template class Any<2000, 2100>;
template class Any<1, 12>;
template class Any<1, 31>;
template class Any<0, 23>;
template class Any<0, 59>;
template class Any<0, 999>;


template class AnyStep<2000, 2100>;
template class AnyStep<1, 12>;
template class AnyStep<1, 31>;
template class AnyStep<0, 23>;
template class AnyStep<0, 59>;
template class AnyStep<0, 999>;
}  // namespace scheduling
