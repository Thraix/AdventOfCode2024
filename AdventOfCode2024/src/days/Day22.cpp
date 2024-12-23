#include "aoc.h"

namespace day22
{
  REGISTER_DAY(day22, std::vector<int>, int64_t);

  REGISTER_TEST_EXAMPLE(day22, ExampleInput, 1, 37990510);
  REGISTER_TEST        (day22, Input,        1, 14273043166);
  REGISTER_TEST_EXAMPLE(day22, ExampleInput, 2, 23);
  REGISTER_TEST        (day22, Input,        2, 1667);

  READ_INPUT(input)
  {
    return Input::ReadInts(input);
  }

  int64_t NextNumber(int64_t currentNum)
  {
    currentNum = ((currentNum * 64) ^ currentNum) % 16777216;
    currentNum = ((currentNum / 32) ^ currentNum) % 16777216;
    currentNum = ((currentNum * 2048) ^ currentNum) % 16777216;
    return currentNum;
  }

  int GetDiffIndex(const std::vector<int>& vec)
  {
    int val = 0;
    for (int i = vec.size() - 5; i < vec.size() - 1; i++)
    {
      val *= 19;
      val += vec[i + 1] - vec[i] + 9;
    }
    return val;
  }

  OUTPUT1(input)
  {
    int64_t sum = 0;
    for (auto num : input)
    {
      for (int i = 0; i < 2000; i++)
      {
        num = NextNumber(num);
      }
      sum += num;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    const int MAX_INDEX = 19 * 19 * 19 * 19;
    std::vector<int> monkeySequences(MAX_INDEX, 0);
    int best = 0;
    for (auto num : input)
    {
      std::vector<bool> alreadyFoundSequence(MAX_INDEX, false);
      std::vector<int> sequence;
      sequence.emplace_back(num % 10);
      for (int i = 0; i < 2000; i++)
      {
        num = NextNumber(num);
        sequence.emplace_back(num % 10);
        if (sequence.size() >= 5)
        {
          int diffIndex = GetDiffIndex(sequence);
          if (!alreadyFoundSequence[diffIndex])
          {
            alreadyFoundSequence[diffIndex] = true;
            monkeySequences[diffIndex] += num % 10;
            if (monkeySequences[diffIndex] > best)
              best = monkeySequences[diffIndex];
          }
        }
      }
    }

    return best;
  }
}
