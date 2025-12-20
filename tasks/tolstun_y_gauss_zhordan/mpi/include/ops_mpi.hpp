#pragma once

#include "tolstun_y_gauss_zhordan/common/include/common.hpp"
#include "task/include/task.hpp"

namespace tolstun_y_gauss_zhordan {

class TolstunYGaussZhordanMPI : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kMPI;
  }
  explicit TolstunYGaussZhordanMPI(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace tolstun_y_gauss_zhordan
