#include "aoc.h"

namespace day10
{
  REGISTER_DAY(day10, Array2D<int>, int32_t);

  REGISTER_TEST_EXAMPLE(day10, ExampleInput, 1, 36);
  REGISTER_TEST        (day10, Input,        1, 786);
  REGISTER_TEST_EXAMPLE(day10, ExampleInput, 2, 81);
  REGISTER_TEST        (day10, Input,        2, 1722);

  READ_INPUT(input)
  {
    return Input::ReadDigitsAsArray2D(input);
  }

  void GetTops(const Array2D<int>& map, Index2D pos, std::set<Index2D>& visited)
  {
    int val = map.Get(pos);
    if (val == 9)
    {
      visited.emplace(pos);
      return;
    }

    std::vector<Index2D> indices{Index2D{-1, 0}, Index2D{1, 0}, Index2D{0, -1}, Index2D{0, 1}};
    for (auto index : indices)
    {
      if (map.IsInside(pos + index) && map.Get(pos + index) == val + 1)
        GetTops(map, pos + index, visited);
    }
  }

  int CountPaths(const Array2D<int>& map, Index2D pos)
  {
    int val = map.Get(pos);
    if (val == 9)
      return 1;

    int paths = 0;
    std::vector<Index2D> indices{Index2D{-1, 0}, Index2D{1, 0}, Index2D{0, -1}, Index2D{0, 1}};
    for (auto index : indices)
    {
      if (map.IsInside(pos + index) && map.Get(pos + index) == val + 1)
        paths += CountPaths(map, pos + index);
    }

    return paths;
  }


  OUTPUT1(input)
  {
    int32_t sum = 0;
    for (int y = 0; y < input.height; y++)
    {
      for (int x = 0; x < input.width; x++)
      {
        if (input.Get(x, y) == 0)
        {
          std::set<Index2D> visited;
          GetTops(input, Index2D{x, y}, visited);
          sum += visited.size();
        }
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int32_t sum = 0;
    for (int y = 0; y < input.height; y++)
    {
      for (int x = 0; x < input.width; x++)
      {
        if (input.Get(x, y) == 0)
        {
          sum += CountPaths(input, Index2D{x, y});
        }
      }
    }
    return sum;
  }
}
