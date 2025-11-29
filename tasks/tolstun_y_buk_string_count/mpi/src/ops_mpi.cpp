#include "tolstun_y_buk_string_count/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>

#include "tolstun_y_buk_string_count/common/include/common.hpp"

namespace tolstun_y_buk_string_count {

TolstunYBukStringCountMPI::TolstunYBukStringCountMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool TolstunYBukStringCountMPI::ValidationImpl() {
  return GetOutput() == 0;
}

bool TolstunYBukStringCountMPI::PreProcessingImpl() {
  GetOutput() = 0;
  return true;
}

// Изолируем
namespace {

int CountBukv(const std::string &s, std::size_t start, std::size_t end) {
  // Считаем количество букв
  int bukv_count = 0;
  for (std::size_t i = start; i < end; ++i) {
    auto sim = static_cast<unsigned char>(s[i]);
    if (std::isalpha(sim) != 0) {
      ++bukv_count;
    }
  }

  return bukv_count;
}

}  // namespace

bool TolstunYBukStringCountMPI::RunImpl() {
  const std::string &stroka = GetInput();
  const std::size_t size_stroka = stroka.size();

  int rank = 0;
  int size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size_stroka == 0) {
    GetOutput() = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }

  std::size_t interval = size_stroka / static_cast<std::size_t>(size);
  std::size_t ostatok = size_stroka % static_cast<std::size_t>(size);

  std::size_t start = (rank * interval) + std::min(static_cast<std::size_t>(rank), ostatok);

  std::size_t end = start + interval;
  if (std::cmp_less(rank, static_cast<int>(ostatok))) {
    end += 1;
  }

  start = std::min(start, size_stroka);
  end = std::min(end, size_stroka);

  start = std::min(start, end);  // защита от пересечения

  int local_count = 0;
  if (start < end) {
    local_count = CountBukv(stroka, start, end);
  }

  int global_count = 0;

  MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Bcast(&global_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

  GetOutput() = global_count;

  MPI_Barrier(MPI_COMM_WORLD);
  return true;
}

bool TolstunYBukStringCountMPI::PostProcessingImpl() {
  return GetOutput() >= 0;
}

}  // namespace tolstun_y_buk_string_count
