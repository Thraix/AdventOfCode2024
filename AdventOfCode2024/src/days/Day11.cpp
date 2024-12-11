#include "aoc.h"

namespace day11
{
  using Stones = std::map<int64_t, int64_t>;
  REGISTER_DAY(day11, Stones, int64_t);

  REGISTER_TEST_EXAMPLE(day11, ExampleInput, 1, 55312);
  REGISTER_TEST        (day11, Input,        1, 189167);
  REGISTER_TEST_EXAMPLE(day11, ExampleInput, 2, 65601038650482);
  REGISTER_TEST        (day11, Input,        2, 225253278506288);

  READ_INPUT(input)
  {
    Stones stones;
    for (auto i : Input::ReadInt64s(input, ' '))
      stones[i]++;
    return stones;
  }

  int logi10(int64_t i)
  {
    int log = 1;
    int64_t base = 10;
    while (i >= base)
    {
      base *= 10;
      log++;
    }
    return log;
  }

  int64_t pow10(int i)
  {
    int64_t pow = 1;

    for(int j = 0; j < i; j++)
      pow *= 10;

    return pow;
  }

  std::pair<int64_t, int64_t> split(int64_t i)
  {
    int log = logi10(i);
    int64_t pow = pow10(log / 2);

    return {i / pow, i % pow};
  }

  void Step(std::map<int64_t, int64_t>& stones)
  {
    std::map<int64_t, int64_t> next;

    for (auto [cur, count] : stones)
    {
      if (cur == 0)
      {
        next[1] += count;
      }
      else if(logi10(cur) % 2 == 0)
      {
        auto [i1, i2] = split(cur);
        next[i1] += count;
        next[i2] += count;
      }
      else
      {
        next[cur * 2024] += count;
      }
    }
    stones = std::move(next);
  }

  int64_t CalculateStones(const std::map<int64_t, int64_t>& stones, int iterations)
  {
    std::map<int64_t, int64_t> cpy = stones;

    for (int i = 0; i < iterations; i++)
      Step(cpy);

    int64_t sum = 0;
    for (auto& [cur, count] : cpy)
    {
      sum += count;
    }
    return sum;
  }

  OUTPUT1(input)
  {
    return CalculateStones(input, 25);
  }

  OUTPUT2(input)
  {
    return CalculateStones(input, 75);
  }
}
