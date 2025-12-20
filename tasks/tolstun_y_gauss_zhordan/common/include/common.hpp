#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace tolstun_y_gauss_zhordan {

using InType = int;
using OutType = int;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace tolstun_y_gauss_zhordan
