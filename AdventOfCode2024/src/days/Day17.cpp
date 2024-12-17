#include "aoc.h"

#include <bitset>

namespace day17
{
  struct Program
  {
    int64_t A;
    int64_t B;
    int64_t C;
    std::vector<int> instructions;
  };

  REGISTER_DAY(day17, Program, int64_t);

  REGISTER_TEST_EXAMPLE(day17, ExampleInput, 1, 0);
  REGISTER_TEST(day17, Input, 1, 0);
  REGISTER_TEST_EXAMPLE(day17, ExampleInput, 2, 117440);
  REGISTER_TEST(day17, Input, 2, 202322348616234);

  READ_INPUT(input)
  {
    Program program;

    std::string str;
    std::getline(input, str);
    std::stringstream ss{str};
    ss >> "Register A: " >> program.A;

    std::getline(input, str);
    ss = std::stringstream{str};
    ss >> "Register B: " >> program.B;

    std::getline(input, str);
    ss = std::stringstream{str};
    ss >> "Register C: " >> program.C;

    std::getline(input, str);
    std::getline(input, str, ' ');
    program.instructions = Input::ReadInts(input, ',');

    return program;
  }

  int64_t Combo(int i, int64_t a, int64_t b, int64_t c)
  {
    if (i <= 3)
      return i;
    if (i == 4)
      return a;
    if (i == 5)
      return b;
    if (i == 6)
      return c;

    std::cerr << "Invalid combo: " << i << std::endl;
    return -1;
  }

  std::vector<int> RunProgram(const Program& program)
  {
    int64_t a = program.A;
    int64_t b = program.B;
    int64_t c = program.C;
    int pc = 0;
    std::vector<int> result;

    while (pc < program.instructions.size())
    {
      int ins = program.instructions[pc];
      int arg = program.instructions[pc + 1];

      if (ins == 0)
        a = a >> Combo(arg, a, b, c);
      else if (ins == 1)
        b = b ^ arg;
      else if (ins == 2)
        b = Combo(arg, a, b, c) % 8;
      else if (ins == 3 && a != 0)
        pc = arg - 2;
      else if (ins == 4)
        b = b ^ c;
      else if (ins == 5)
        result.emplace_back(Combo(arg, a, b, c) % 8);
      else if(ins == 6)
        b = a >> Combo(arg, a, b, c);
      else if(ins == 7)
        c = a >> Combo(arg, a, b, c);

      pc += 2;
    }
    return result;
  }

  std::vector<int64_t> GetDiffPeriodicity(const std::vector<int64_t>& ints)
  {
    if (ints.empty())
      return {};

    const int MIN_LOOP_COUNT = 3;
    if ((ints.size() - 1) % MIN_LOOP_COUNT != 0)
      return {};

    std::vector<int64_t> diffs;
    for (int i = 0; i < ints.size() - 1; i++)
    {
      diffs.emplace_back(ints[i + 1] - ints[i]);
    }

    int loopSize = ints.size() / MIN_LOOP_COUNT;
    for (int i = 0; i < MIN_LOOP_COUNT - 1; i++)
    {
      for (int j = 0; j < loopSize; j++)
      {
        if (diffs[i * loopSize + j] != diffs[i * loopSize + loopSize + j])
        {
          return {};
        }
      }
    }

    return {diffs.begin(), diffs.begin() + loopSize};
  }

  OUTPUT1(input)
  {
    std::vector<int> result = RunProgram(input);
    std::cout << result << std::endl;
    return 0;
  }

  OUTPUT2(input)
  {
    int64_t aRegister = 0;
    int currentDepth = 0;
    std::vector<int64_t> diffs{1};
    std::vector<int64_t> aRegisterSolution{};
    Program program = input;
    int offset = 0;
    while (true)
    {
      program.A = aRegister;
      std::vector<int> vec = RunProgram(program);

      if (vec == input.instructions)
      {
        return aRegister;
      }

      if (vec.size() > currentDepth && vec[currentDepth] == program.instructions[currentDepth])
      {
        aRegisterSolution.emplace_back(aRegister);
      }

      std::vector<int64_t> loop = GetDiffPeriodicity(aRegisterSolution);
      if (!loop.empty())
      {
        aRegister = aRegisterSolution[0];
        diffs = loop;
        aRegisterSolution.clear();
        offset = 0;
        currentDepth++;
      }
      else
      {
        aRegister += diffs[offset % diffs.size()];
        offset++;
      }
    }
    return 0;
  }
}
