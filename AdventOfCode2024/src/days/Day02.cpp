#include "aoc.h"

namespace day02
{
  REGISTER_DAY(day02, std::vector<std::vector<int>>, int);

  REGISTER_TEST_EXAMPLE(day02, ExampleInput, 1, 2);
  REGISTER_TEST        (day02, Input,        1, 549);
  REGISTER_TEST_EXAMPLE(day02, ExampleInput, 2, 4);
  REGISTER_TEST        (day02, Input,        2, 589);

  READ_INPUT(input)
  {
    std::vector<std::vector<int>> list;
    std::string str;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      list.emplace_back(Input::ReadInts(ss, ' '));
    }
    return list;
  }

  bool Validate(const std::vector<int>& vec)
  {
    bool lt = vec[0] < vec[1];
    for (int j = 1; j < vec.size(); j++)
    {
      int diff = std::abs(vec[j] - vec[j - 1]);
      if (vec[j - 1] < vec[j] != lt || diff < 1 || diff > 3)
      {
        return false;
      }
    }
    return true;
  }

  bool ValidateWithout(const std::vector<int>& vec, int index)
  {
    std::vector<int> cpy = vec;
    if(index < vec.size())
      cpy.erase(cpy.begin() + index);
    return Validate(cpy);
  }

  OUTPUT1(input)
  {
    int sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      if (Validate(input[i]))
      {
        sum++;
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      for (int j = 0; j < input[i].size() + 1; j++)
      {
        if (ValidateWithout(input[i], j))
        {
          sum++;
          break;
        }
      }
    }
    return sum;
  }
}
