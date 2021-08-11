#pragma once

#include <vector>
#include <memory>

#include <conditions.h>

namespace scheduling {

class Combiner final : public ICondition {
  typedef std::unique_ptr<ICondition> condition_ptr;
  typedef std::vector<condition_ptr> Conditions;


  Conditions _conditions;
  Conditions::iterator _heapEnd;

  static bool heap_cmp(const condition_ptr& l, const condition_ptr& r);

  void makeHeap();

 public:
  Combiner();
  // todo: move to constructor??
  void emplace(condition_ptr&& ptr);

  // Inherited via ICondition
  virtual void init(const Unit current) override;
  virtual void reset() override;
  virtual operator bool() const override;
  virtual Unit operator*() const override;
  virtual ICondition& operator++() override;
};

}  // namespace scheduling