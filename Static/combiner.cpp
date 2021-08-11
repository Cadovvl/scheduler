#include "pch.h"
#include "combiner.h"

#include <algorithm>

namespace scheduling {

bool Combiner::heap_cmp(const condition_ptr& l, const condition_ptr& r) {
  return **l > **r;
}

void Combiner::makeHeap() {
  // filter conditions without value
  _heapEnd = std::partition(_conditions.begin(), _conditions.end(),
                            [](const auto& c) { return bool(*c); });

  // heap from what rest
  if (_conditions.begin() != _heapEnd) {
    std::make_heap(_conditions.begin(), _heapEnd, heap_cmp);
  }
}

Combiner::Combiner(): _heapEnd(_conditions.begin()) {}

void Combiner::emplace(condition_ptr&& ptr) { _conditions.emplace_back(std::move(ptr)); }

void Combiner::init(const Unit current) {
  for (auto& c : _conditions) {
    c->init(current);
  }
  makeHeap();
}

void Combiner::reset() {
  for (auto& c : _conditions) {
    c->reset();
  }
  makeHeap();
}

Combiner::operator bool() const { return _heapEnd != _conditions.begin(); }

Unit Combiner::operator*() const { return **_conditions.front(); }

Combiner& Combiner::operator++() {
  Unit val = **_conditions.front();
  while (_conditions.begin() != _heapEnd && val == **_conditions.front()) {
    std::pop_heap(_conditions.begin(), _heapEnd, heap_cmp);
    --_heapEnd;
    ++(**_heapEnd);
    if (**_heapEnd) {
      ++_heapEnd;
      std::push_heap(_conditions.begin(), _heapEnd, heap_cmp);
    }
  }
  return *this;
}

}  // namespace scheduling
