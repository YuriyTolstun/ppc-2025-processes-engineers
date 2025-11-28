#include "tolstun_y_buk_string_count/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "tolstun_y_buk_string_count/common/include/common.hpp"
#include "util/include/util.hpp"

namespace tolstun_y_buk_string_count {

TolstunYBukStringCountMPI::TolstunYBukStringCountMPI(const InType &in)
{
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool TolstunYBukStringCountMPI::ValidationImpl() 
{
  return GetOutput() == 0;
}

bool TolstunYBukStringCountMPI::PreProcessingImpl() 
{
  GetOutput() = 0;
  return true;
}

//Изолируем
namespace 
{

  int CountBukv(const std::string &s, std::size_t start, std::size_t end) 
  {
    
    //Считаем количество букв
    int bukvCount=0;
    for(std::size_t i = start; i < end; ++i)
    {
      auto sim = static_cast<unsigned char>(s[i]);
      if(std::isalpha(sim))
      {
        ++bukvCount;
      }
    }

    return bukvCount;
  }

} 

bool TolstunYBukStringCountMPI::RunImpl() 
{
  const std::string &stroka = GetInput();
  const std::size_t sizeStroka = stroka.size();

  int rank = 0;
  int size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (sizeStroka == 0) 
  {
    GetOutput() = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }

  
  std::size_t interval = sizeStroka / static_cast<std::size_t>(size);
  std::size_t ostatok = sizeStroka % static_cast<std::size_t>(size);

  std::size_t start = rank * interval + std::min(static_cast<std::size_t>(rank), ostatok);
  
  std::size_t end = start + interval;
  if (static_cast<std::size_t>(rank) < ostatok)
  {
    end += 1;
  }

  start = std::min(start, sizeStroka);
  end = std::min(end, sizeStroka);
  
  start = std::min(start, end); // защита от пересечения

  int localCount = 0;
  if (start < end) {
      localCount = CountBukv(stroka, start, end);
  }

  int globalCount = 0;

  MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Bcast(&globalCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

  GetOutput() = globalCount;

  MPI_Barrier(MPI_COMM_WORLD);
  return true;
}

bool TolstunYBukStringCountMPI::PostProcessingImpl() 
{
  return GetOutput() >= 0;
}

}  // namespace tolstun_y_buk_string_count
