#include "tolstun_y_buk_string_count/seq/include/ops_seq.hpp"

#include <cctype>
#include <string>

#include "tolstun_y_buk_string_count/common/include/common.hpp"
namespace tolstun_y_buk_string_count {

TolstunYBukStringCountSEQ::TolstunYBukStringCountSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool TolstunYBukStringCountSEQ::ValidationImpl() {
  return (GetOutput() == 0);
}

bool TolstunYBukStringCountSEQ::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

bool TolstunYBukStringCountSEQ::RunImpl() {
  const std::string &stroka = GetInput();

  // Считаем количество букв
  int bukv_count = 0;
  for (char sim : stroka) {
    if (std::isalpha(sim) != 0) {
      bukv_count++;
    }
  }

  GetOutput() = bukv_count;
  return true;
}

bool TolstunYBukStringCountSEQ::PostProcessingImpl() {
  return GetOutput() >= 0;
}

}  // namespace tolstun_y_buk_string_count
