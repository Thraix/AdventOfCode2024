#include "aoc.h"

namespace day04
{
  REGISTER_DAY(day04, Array2D<char>, int32_t);

  REGISTER_TEST_EXAMPLE(day04, ExampleInput, 1, 18);
  REGISTER_TEST        (day04, Input,        1, 2543);
  REGISTER_TEST_EXAMPLE(day04, ExampleInput, 2, 9);
  REGISTER_TEST        (day04, Input,        2, 1930);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  OUTPUT1(input)
  {
    int32_t sum = 0;
    for (int y = 0; y < input.height; y++)
    {
      for (int x = 0; x < input.width; x++)
      {
        if (y < input.height -3 && input.Get(x, y) == 'X' && input.Get(x, y + 1) == 'M' && input.Get(x, y + 2) == 'A' && input.Get(x, y + 3) == 'S')
          sum++;
        if (y < input.height -3 && input.Get(x, y) == 'S' && input.Get(x, y + 1) == 'A' && input.Get(x, y + 2) == 'M' && input.Get(x, y + 3) == 'X')
          sum++;
        if (x < input.width - 3 && input.Get(x, y) == 'X' && input.Get(x + 1, y) == 'M' && input.Get(x + 2, y) == 'A' && input.Get(x + 3, y) == 'S')
          sum++;
        if (x < input.width - 3 && input.Get(x, y) == 'S' && input.Get(x + 1, y) == 'A' && input.Get(x + 2, y) == 'M' && input.Get(x + 3, y) == 'X')
          sum++;
        if (y < input.height -3 && x < input.width -3 && input.Get(x, y) == 'X' && input.Get(x + 1, y + 1) == 'M' && input.Get(x + 2, y + 2) == 'A' && input.Get(x + 3, y + 3) == 'S')
          sum++;
        if (y < input.height -3 && x < input.width -3 && input.Get(x, y) == 'S' && input.Get(x + 1, y + 1) == 'A' && input.Get(x + 2, y + 2) == 'M' && input.Get(x + 3, y +3 ) == 'X')
          sum++;
        if (y < input.height -3 && x < input.width -3 && input.Get(x + 3, y) == 'X' && input.Get(x + 2, y + 1) == 'M' && input.Get(x + 1, y + 2) == 'A' && input.Get(x, y + 3) == 'S')
          sum++;
        if (y < input.height -3 && x < input.width -3 && input.Get(x + 3, y) == 'S' && input.Get(x + 2, y + 1) == 'A' && input.Get(x + 1, y + 2) == 'M' && input.Get(x, y +3 ) == 'X')
          sum++;
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int32_t sum = 0;
    for (int y = 0; y < input.height - 2; y++)
    {
      for (int x = 0; x < input.width - 2; x++)
      {
        if (input.Get(x, y) == 'M' && input.Get(x, y + 2) == 'M' && input.Get(x + 1, y + 1) == 'A' && input.Get(x + 2, y) == 'S' && input.Get(x + 2, y + 2) == 'S')
          sum++;
        if (input.Get(x, y) == 'M' && input.Get(x, y + 2) == 'S' && input.Get(x + 1, y + 1) == 'A' && input.Get(x + 2, y) == 'M' && input.Get(x + 2, y + 2) == 'S')
          sum++;
        if (input.Get(x, y) == 'S' && input.Get(x, y + 2) == 'M' && input.Get(x + 1, y + 1) == 'A' && input.Get(x + 2, y) == 'S' && input.Get(x + 2, y + 2) == 'M')
          sum++;
        if (input.Get(x, y) == 'S' && input.Get(x, y + 2) == 'S' && input.Get(x + 1, y + 1) == 'A' && input.Get(x + 2, y) == 'M' && input.Get(x + 2, y + 2) == 'M')
          sum++;
      }
    }
    return sum;
  }
}
