#include "tolstun_y_buk_string_count/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "tolstun_y_buk_string_count/common/include/common.hpp"
#include "util/include/util.hpp"

namespace tolstun_y_buk_string_count {

TolstunYBukStringCountSEQ::TolstunYBukStringCountSEQ(const InType &in) 
{
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool TolstunYBukStringCountSEQ::ValidationImpl() 
{
  return (GetOutput() == 0);
}

bool TolstunYBukStringCountSEQ::PreProcessingImpl() 
{
  GetOutput() = 0;
  return true;
}

bool TolstunYBukStringCountSEQ::RunImpl() 
{
  const std::string &stroka = GetInput();
  
  //Считаем количество букв
  int bukvCount=0;
  for(char sim : stroka)
  {
    if(std::isalpha(sim))
    {
      bukvCount++;
    }
  }


  GetOutput() = bukvCount;
  return true;
}

bool TolstunYBukStringCountSEQ::PostProcessingImpl() 
{
  return GetOutput() >= 0;
}

}  // namespace tolstun_y_buk_string_count
