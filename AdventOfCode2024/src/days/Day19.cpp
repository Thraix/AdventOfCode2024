#include "aoc.h"

namespace day19
{
  struct Towels
  {
    std::vector<std::string> patterns;
    std::vector<std::string> towels;
  };

  REGISTER_DAY(day19, Towels, int64_t);

  REGISTER_TEST_EXAMPLE(day19, ExampleInput, 1, 6);
  REGISTER_TEST        (day19, Input,        1, 353);
  REGISTER_TEST_EXAMPLE(day19, ExampleInput, 2, 16);
  REGISTER_TEST        (day19, Input,        2, 880877787214477);

  READ_INPUT(input)
  {
    Towels strings;
    std::string str;
    std::getline(input, str);
    std::stringstream ss{str};
    while (std::getline(ss, str, ','))
    {
      if (str.front() == ' ')
        str = str.substr(1);
      strings.patterns.emplace_back(str);
    }

    std::getline(input, str);

    while (std::getline(input, str))
    {
      strings.towels.emplace_back(str);
    }
    return strings;
  }

  bool HasCombination(const std::string& towel, const std::vector<std::string>& patterns, int offset, std::set<int>& memoization)
  {
    if (offset == towel.size())
      return true;

    if (memoization.count(offset) != 0)
      return false;

    for (auto& other : patterns)
    {
      if (Helper::StartsWith(std::string_view(towel.c_str() + offset, towel.size() - offset), other))
      {
        if (HasCombination(towel, patterns, offset + other.size(), memoization))
        {
          return true;
        }
      }
    }

    memoization.emplace(offset);
    return false;
  }

  int64_t AmountOfCombinations(const std::string& towel, const std::vector<std::string>& patterns, int offset, std::map<int, int64_t>& memoization)
  {
    if (offset == towel.size())
      return 1;

    auto it = memoization.find(offset);
    if (it != memoization.end())
      return it->second;

    int64_t amount = 0;
    for (auto& other : patterns)
    {
      if (Helper::StartsWith(std::string_view(towel.c_str() + offset, towel.size() - offset), other))
      {
        amount += AmountOfCombinations(towel, patterns, offset + other.size(), memoization);
      }
    }
    memoization[offset] = amount;
    return amount;
  }

  OUTPUT1(input)
  {
    int64_t sum = 0;

    for (auto& towel : input.towels)
    {
      std::set<int> memoization;
      if (HasCombination(towel, input.patterns, 0, memoization))
        sum++;
    }

    return sum;
  }

  OUTPUT2(input)
  {
    int64_t sum = 0;

    for (auto& towel : input.towels)
    {
      std::map<int, int64_t> memoization;
      sum += AmountOfCombinations(towel, input.patterns, 0, memoization);
    }

    return sum;
  }
}
