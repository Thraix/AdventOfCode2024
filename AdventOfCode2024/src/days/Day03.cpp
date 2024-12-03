#include "aoc.h"

namespace day03
{
  REGISTER_DAY(day03, std::string, int32_t);

  REGISTER_TEST_EXAMPLE(day03, ExampleInput, 1, 161);
  REGISTER_TEST        (day03, Input,        1, 178794710);
  REGISTER_TEST_EXAMPLE(day03, ExampleInput, 2, 48);
  REGISTER_TEST        (day03, Input,        2, 76729637);

  READ_INPUT(input)
  {
    return Input::Read(input);
  }

  int32_t Mult(const std::string& str)
  {
    int i, j;
    std::stringstream ss{str};
    ss >> "mul(" >> i >> "," >> j >> ")";
    return i * j;
  }

  OUTPUT1(input)
  {
    int32_t sum = 0;
    std::vector<std::string> matches = Helper::GetAllRegexMatches(input, "mul\\([0-9]*,[0-9]*\\)");
    for (auto& str : matches)
      sum += Mult(str);
    return sum;
  }

  OUTPUT2(input)
  {
    int32_t sum = 0;
    bool mul = true;
    std::vector<std::string> matches = Helper::GetAllRegexMatches(input, "(mul\\([0-9]*,[0-9]*\\)|do\\(\\)|don\'t\\(\\))");
    for (auto& str : matches)
    {
      if(str == "do()")
        mul = true;
      else if (str == "don\'t()")
        mul = false;
      else if (mul)
        sum += Mult(str);
    }
    return sum;
  }
}
