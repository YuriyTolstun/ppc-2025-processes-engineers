#pragma once

#include "tolstun_y_gauss_zhordan/common/include/common.hpp"
#include "task/include/task.hpp"

namespace tolstun_y_gauss_zhordan {

class TolstunYGaussZhordanSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit TolstunYGaussZhordanSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace tolstun_y_gauss_zhordan
