#include "aoc.h"

namespace day25
{
  struct KeysLocks
  {
    int height{0};
    std::vector<std::vector<int>> keys;
    std::vector<std::vector<int>> locks;
  };

  REGISTER_DAY(day25, KeysLocks, int);

  REGISTER_TEST_EXAMPLE(day25, ExampleInput, 1, 3);
  REGISTER_TEST        (day25, Input,        1, 3663);
  REGISTER_TEST_EXAMPLE(day25, ExampleInput, 2, 0);
  REGISTER_TEST        (day25, Input,        2, 0);

  READ_INPUT(input)
  {
    KeysLocks keysLocks;

    bool next = true;
    while (next)
    {
      next = false;
      std::string str;
      std::vector<int> lockOrKey;
      bool lock = true;
      int height = 0;
      while (std::getline(input, str))
      {
        next = true;
        if (str.empty())
          break;

        if (lockOrKey.empty())
        {
          if (str[0] == '.')
            lock = false;

          lockOrKey = std::vector<int>(str.size(), 0);
        }

        for (int i = 0; i < str.size();i++)
        {
          if(str[i] == '#')
            lockOrKey[i]++;
        }
        height++;
      }

      if (keysLocks.height == 0)
        keysLocks.height = height;

      if (!lockOrKey.empty())
      {
        if (lock)
          keysLocks.locks.emplace_back(lockOrKey);
        else
          keysLocks.keys.emplace_back(lockOrKey);
      }
    }

    return keysLocks;
  }

  OUTPUT1(input)
  {
    int64_t sum = 0;
    for (const auto& key : input.keys)
    {
      for (const auto& lock : input.locks)
      {
        bool matches = true;
        for (int i = 0; i < key.size(); i++)
        {
          if (key[i] + lock[i] > input.height)
          {
            matches = false;
            break;
          }
        }

        if (matches)
          sum++;
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    std::cout << "  ___                      ___" << std::endl;
    std::cout << " (o o)                    (o o)" << std::endl;
    std::cout << "(  V  ) MERRY CHRISTMAS! (  V  )" << std::endl;
    std::cout << "--m-m----------------------m-m--" << std::endl;
    return 0;
  }
}
