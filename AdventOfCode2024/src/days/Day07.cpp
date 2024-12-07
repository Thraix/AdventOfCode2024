#include "aoc.h"

namespace day07
{
  struct Data
  {
    int64_t result;
    std::vector<int64_t> terms;

  };
  REGISTER_DAY(day07, std::vector<Data>, int64_t);

  REGISTER_TEST_EXAMPLE(day07, ExampleInput, 1, 3749);
  REGISTER_TEST        (day07, Input,        1, 945512582195);
  REGISTER_TEST_EXAMPLE(day07, ExampleInput, 2, 11387);
  REGISTER_TEST        (day07, Input,        2, 271691107779347);

  READ_INPUT(input)
  {
    std::vector<Data> datas;
    std::string str;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      Data data;
      ss >> data.result >> ": ";
      while (std::getline(ss, str, ' '))
      {
        data.terms.emplace_back(std::stoll(str));
      }
      datas.emplace_back(data);
    }
    return datas;
  }

  bool CanCalculateResultP1(const Data& data, int index, int64_t current)
  {
    if (index >= data.terms.size())
      return data.result == current;
    if (index == 0)
      return CanCalculateResultP1(data, index + 1, data.terms[index]);

    if (CanCalculateResultP1(data, index + 1, current + data.terms[index]))
      return true;
    else if (CanCalculateResultP1(data, index + 1, current * data.terms[index]))
      return true;
    return false;
  }

  int64_t Concat(int64_t i1, int64_t i2)
  {
    int64_t base = 10;
    while (i2 >= base)
      base *= 10;
    return i1 * base + i2;
  }

  bool CanCalculateResultP2(const Data& data, int index, int64_t current)
  {
    if (index >= data.terms.size())
      return data.result == current;
    if (index == 0)
      return CanCalculateResultP2(data, index + 1, data.terms[index]);

    if (CanCalculateResultP2(data, index + 1, current + data.terms[index]))
      return true;
    else if (CanCalculateResultP2(data, index + 1, current * data.terms[index]))
      return true;
    else if (CanCalculateResultP2(data, index + 1, Concat(current, data.terms[index])))
      return true;
    return false;
  }

  OUTPUT1(input)
  {
    int64_t sum = 0;
    for (const auto& data : input)
    {
      if (CanCalculateResultP1(data, 0, 0))
        sum += data.result;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int64_t sum = 0;
    for (const auto& data : input)
    {
      if (CanCalculateResultP2(data, 0, 0))
        sum += data.result;
    }
    return sum;
  }
}
