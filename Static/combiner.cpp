#include "pch.h"
#include "combiner.h"

#include <algorithm>

namespace scheduling {

template <bool Reversed>
bool CombinerBase<Reversed>::heap_cmp(const condition_ptr& l,
                                      const condition_ptr& r) {
  if constexpr (Reversed) {
    return **l < **r;
  } else {
    return **l > **r;
  }
}

template <bool Reversed>
void CombinerBase<Reversed>::makeHeap() {
  // filter conditions without value
  _heapEnd = std::partition(_conditions.begin(), _conditions.end(),
                            [](const auto& c) { return bool(*c); });

  // heap from what rest
  if (_conditions.begin() != _heapEnd) {
    std::make_heap(_conditions.begin(), _heapEnd, heap_cmp);
  }
}

template <bool Reversed>
CombinerBase<Reversed>::CombinerBase<Reversed>()
    : _heapEnd(_conditions.begin()) {}

template <bool Reversed>
void CombinerBase<Reversed>::emplace(condition_ptr&& ptr) {
  _conditions.emplace_back(std::move(ptr));
}

template <bool Reversed>
void CombinerBase<Reversed>::init(const Unit current) {
  for (auto& c : _conditions) {
    c->init(current);
  }
  makeHeap();
}

template <bool Reversed>
void CombinerBase<Reversed>::reset() {
  for (auto& c : _conditions) {
    c->reset();
  }
  makeHeap();
}

template <bool Reversed>
CombinerBase<Reversed>::operator bool() const {
  return _heapEnd != _conditions.begin();
}

template <bool Reversed>
Unit CombinerBase<Reversed>::operator*() const { return **_conditions.front(); }

template <bool Reversed>
CombinerBase<Reversed>& CombinerBase<Reversed>::operator++() {
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


template class CombinerBase<true>;
template class CombinerBase<false>;

}  // namespace scheduling
