#include <gtest/gtest.h>

#include "tolstun_y_buk_string_count/common/include/common.hpp"
#include "tolstun_y_buk_string_count/mpi/include/ops_mpi.hpp"
#include "tolstun_y_buk_string_count/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace tolstun_y_buk_string_count {

class TolstunYRunPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  InType input_data_;
  OutType expected_output_{};

  void SetUp() override 
  {
    const size_t bukvCount = 50'000'000;
    std::string stroka(bukvCount, 'a');
    input_data_=stroka;
    expected_output_=bukvCount;
  }

  bool CheckTestOutputData(OutType &output_data) final 
  {
    return output_data == expected_output_;
  }

  InType GetTestInputData() final 
  {
    return input_data_;
  }
};

TEST_P(TolstunYRunPerfTest, RunPerfModes) 
{
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, TolstunYBukStringCountMPI, TolstunYBukStringCountSEQ>(PPC_SETTINGS_tolstun_y_buk_string_count);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = TolstunYRunPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, TolstunYRunPerfTest, kGtestValues, kPerfTestName);

}  // namespace tolstun_y_buk_string_count
