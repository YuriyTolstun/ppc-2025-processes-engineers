#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace tolstun_y_buk_string_count {

using InType = std::string;
using OutType = int;
using TestType = std::tuple<InType, OutType>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace tolstun_y_buk_string_count
