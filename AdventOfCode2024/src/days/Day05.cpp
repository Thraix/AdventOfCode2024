#include "aoc.h"

namespace day05
{
  struct Manual
  {
    std::vector<std::pair<int, int>> pairs;
    std::vector<std::vector<int>> pages;
  };
  REGISTER_DAY(day05, Manual, int32_t);

  REGISTER_TEST_EXAMPLE(day05, ExampleInput, 1, 143);
  REGISTER_TEST        (day05, Input,        1, 5964);
  REGISTER_TEST_EXAMPLE(day05, ExampleInput, 2, 123);
  REGISTER_TEST        (day05, Input,        2, 4719);

  READ_INPUT(input)
  {
    std::vector<std::pair<int, int>> pairs;
    std::string str;
    while (std::getline(input, str))
    {
      if (str.empty())
        break;

      int i, j;
      std::stringstream ss{str};
      ss >> i >> "|" >> j;
      pairs.emplace_back(i, j);
    }
    std::vector<std::vector<int>> pages;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      pages.emplace_back(Input::ReadInts(ss, ','));
    }
    return Manual{pairs, pages};
  }

  std::pair<bool, int> IsValid(const std::vector<int>& input, const std::vector<std::pair<int, int>>& pairs)
  {
    for (int i = 0; i < input.size() - 1; i++)
    {
      for (const auto& pair : pairs)
      {
        if (input[i] == pair.second && input[i + 1] == pair.first)
        {
          return {false, i};
        }
      }
    }
    return {true, 0};
  }

  std::vector<int> Sort(std::vector<int> input, const std::vector<std::pair<int, int>>& pairs)
  {
    bool valid;
    do
    {
      auto [res, index] = IsValid(input, pairs);
      if (!res)
        std::swap(input[index], input[index + 1]);
      valid = res;
    } while (!valid);

    return input;
  }

  OUTPUT1(input)
  {
    int32_t sum = 0;
    for (const auto& man : input.pages)
    {
      if (IsValid(man, input.pairs).first)
        sum += man[man.size() / 2];
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int32_t sum = 0;
    for (const auto& man : input.pages)
    {
      if (!IsValid(man, input.pairs).first)
      {
        auto newMan = Sort(man, input.pairs);
        sum += newMan[newMan.size() / 2];
      }
    }
    return sum;
  }
}
