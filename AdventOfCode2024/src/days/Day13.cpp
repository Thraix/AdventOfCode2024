#include "aoc.h"

namespace day13
{
  struct ClawMachine
  {
    int64_t buttonAX;
    int64_t buttonAY;
    int64_t buttonBX;
    int64_t buttonBY;
    int64_t prizeX;
    int64_t prizeY;
  };

  REGISTER_DAY(day13, std::vector<ClawMachine>, int64_t);

  REGISTER_TEST_EXAMPLE(day13, ExampleInput, 1, 480);
  REGISTER_TEST        (day13, Input,        1, 26005);
  REGISTER_TEST_EXAMPLE(day13, ExampleInput, 2, 875318608908);
  REGISTER_TEST        (day13, Input,        2, 105620095782547);

  READ_INPUT(input)
  {
    std::vector<ClawMachine> clawMachines;
    std::string str;
    while (std::getline(input, str))
    {
      ClawMachine machine;
      std::stringstream ss{str};
      ss >> "Button A: X+" >> machine.buttonAX >> ", Y+" >> machine.buttonAY;

      std::getline(input, str);
      ss = std::stringstream{str};
      ss >> "Button B: X+" >> machine.buttonBX >> ", Y+" >> machine.buttonBY;

      std::getline(input, str);
      ss = std::stringstream{str};
      ss >> "Prize: X=" >> machine.prizeX >> ", Y=" >> machine.prizeY;
      std::getline(input, str);
      clawMachines.emplace_back(machine);
    }
    return clawMachines;
  }

  int64_t GetCost(const ClawMachine& machine)
  {
    // Solve
    // aX * a + bX * b = pX
    // aY * a + bY * b = pY

    // aY * aX * a + aY * bX * b = aY * pX
    // -aY * aX * a + -aX * bY * b = -aX * pY

    // (aY * bX * b) + (-aX * bY * b) = aY * pX + (-aX * pY)
    // (aY * bX - aX * bY) * b = aY * pX - aX * pY
    // b = (aY * pX - aX * pY) / (aY * bX - aX * bY)
    // a = (pX - bX * b) / aX

    int64_t aYbX = machine.buttonAY * machine.buttonBX;
    int64_t aYpX = machine.buttonAY * machine.prizeX;

    int64_t aXbY = machine.buttonAX * machine.buttonBY;
    int64_t aXpY = machine.buttonAX * machine.prizeY;

    if (aYbX - aXbY == 0)
    {
      std::cerr << "Multiple solutions" << std::endl;
    }
    else if ((aYpX - aXpY) % (aYbX - aXbY) == 0) // integer solution exists
    {
      int64_t b = (aYpX - aXpY) / (aYbX - aXbY);
      int64_t a = (machine.prizeX - b * machine.buttonBX) / machine.buttonAX;
      if (a >= 0 && b >= 0)
        return b + a * 3;
    }
    return 0; // No solution
  }

  OUTPUT1(input)
  {
    int64_t sum = 0;
    for (auto& machine : input)
    {
      sum += GetCost(machine);
    }

    return sum;
  }

  OUTPUT2(input)
  {
    int64_t sum = 0;
    for (auto machine : input)
    {
      machine.prizeX += 10000000000000;
      machine.prizeY += 10000000000000;
      sum += GetCost(machine);
    }

    return sum;
  }
}
