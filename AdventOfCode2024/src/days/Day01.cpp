#include "aoc.h"

namespace day01
{
  using InputType = std::pair<std::vector<int>, std::vector<int>>;
  REGISTER_DAY(day01, InputType, int);

  REGISTER_TEST_EXAMPLE(day01, ExampleInput, 1, 11);
  REGISTER_TEST        (day01, Input,        1, 1834060);
  REGISTER_TEST_EXAMPLE(day01, ExampleInput, 2, 31);
  REGISTER_TEST        (day01, Input,        2, 21607792);

  READ_INPUT(input)
  {
    std::vector<int> aVec, bVec;
    std::string str;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      int a, b;
      ss >> a >> b;
      aVec.emplace_back(a);
      bVec.emplace_back(b);
    }
    return {aVec, bVec};
  }

  OUTPUT1(input)
  {
    std::vector<int> copyA = input.first;
    std::vector<int> copyB = input.second;
    std::sort(copyA.begin(), copyA.end());
    std::sort(copyB.begin(), copyB.end());
    int sum = 0;
    for (int i = 0; i < input.first.size(); i++)
    {
      sum += std::abs(copyA[i] - copyB[i]);
    }

    return sum;
  }

  OUTPUT2(input)
  {
    std::map<int, int> idCount;
    for (int i : input.second)
    {
      idCount[i]++;
    }

    int sum = 0;
    for (int i : input.first)
    {
      sum += i * idCount[i];
    }

    return sum;
  }
}
