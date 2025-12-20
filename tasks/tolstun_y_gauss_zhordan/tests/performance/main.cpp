#include <gtest/gtest.h>

#include "tolstun_y_gauss_zhordan/common/include/common.hpp"
#include "tolstun_y_gauss_zhordan/mpi/include/ops_mpi.hpp"
#include "tolstun_y_gauss_zhordan/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace tolstun_y_gauss_zhordan {

class TolstunYGaussZhordanPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};

  void SetUp() override {
    input_data_ = kCount_;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return input_data_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(TolstunYGaussZhordanPerfTest, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, TolstunYGaussZhordanMPI, TolstunYGaussZhordanSEQ>(PPC_SETTINGS_tolstun_y_gauss_zhordan);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = TolstunYGaussZhordanPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, TolstunYGaussZhordanPerfTest, kGtestValues, kPerfTestName);

}  // namespace tolstun_y_gauss_zhordan
