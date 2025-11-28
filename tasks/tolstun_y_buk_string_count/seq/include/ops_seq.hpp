#pragma once

#include "tolstun_y_buk_string_count/common/include/common.hpp"
#include "task/include/task.hpp"

namespace tolstun_y_buk_string_count {

class TolstunYBukStringCountSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit TolstunYBukStringCountSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace tolstun_y_buk_string_count
