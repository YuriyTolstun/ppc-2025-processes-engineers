#include <gtest/gtest.h>
#include <stb/stb_image.h>

#include <array>
#include <cstddef>
#include <string>
#include <tuple>

#include "tolstun_y_buk_string_count/common/include/common.hpp"
#include "tolstun_y_buk_string_count/mpi/include/ops_mpi.hpp"
#include "tolstun_y_buk_string_count/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace tolstun_y_buk_string_count {

class TolstunYRunFuncTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    const auto &input = std::get<0>(test_param);
    const auto &expect = std::get<1>(test_param);
    return ppc::util::test::SanitizeToken(input) + "_expect_" + std::to_string(expect);
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    input_data_ = std::get<0>(params);
    expected_output_ = std::get<1>(params);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == expected_output_;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType expected_output_{};
};

namespace {

const std::array<TestType, 6> kTestParam = {
    TestType{InType(""), 0},
    TestType{InType("12/3'asdg/231"), 4},
    TestType{InType("ksdf3  ksd 00a"), 8},
    TestType{InType("453              sdf"), 3},
    TestType{InType("dfs\n       23\t  aab"), 6},
    TestType{InType(" 1323 0200 ///``` "), 0},
};

TEST_P(TolstunYRunFuncTests, BukStringCount) {
  ExecuteTest(GetParam());
}

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<TolstunYBukStringCountMPI, InType>(kTestParam, PPC_SETTINGS_tolstun_y_buk_string_count),
    ppc::util::AddFuncTask<TolstunYBukStringCountSEQ, InType>(kTestParam, PPC_SETTINGS_tolstun_y_buk_string_count));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = TolstunYRunFuncTests::PrintFuncTestName<TolstunYRunFuncTests>;

INSTANTIATE_TEST_SUITE_P(BukvCountTests, TolstunYRunFuncTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace tolstun_y_buk_string_count
