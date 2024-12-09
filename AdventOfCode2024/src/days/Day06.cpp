#include "aoc.h"

namespace day06
{
  REGISTER_DAY(day06, Array2D<char>, int32_t);

  REGISTER_TEST_EXAMPLE(day06, ExampleInput, 1, 41);
  REGISTER_TEST        (day06, Input,        1, 4776);
  REGISTER_TEST_EXAMPLE(day06, ExampleInput, 2, 6);
  REGISTER_TEST        (day06, Input,        2, 1586);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  bool TestObstacle(const Array2D<char>& map, Index2D obstacle, Index2D startPos, Index2D startDir)
  {
    std::set<std::pair<Index2D, Index2D>> visited;
    Index2D index = startPos;
    Index2D dir = startDir;

    while (map.IsInside(index) && visited.find(std::pair{index, dir}) == visited.end())
    {
      visited.emplace(index, dir);
      Index2D newIndex{index.x + dir.x, index.y + dir.y};
      if ((map.IsInside(newIndex) && map.Get(newIndex) == '#') || newIndex == obstacle)
      {
        Index2D newDir{-dir.y, dir.x};
        dir = newDir;
      }
      else
      {
        index = newIndex;
      }
    }
    return visited.find(std::pair{index, dir}) != visited.end();
  }

  OUTPUT1(input)
  {
    std::set<Index2D> visited;
    Index2D index = input.Find('^');
    Index2D dir{0, -1};
    while (input.IsInside(index))
    {
      visited.emplace(index);
      Index2D newIndex{index.x + dir.x, index.y + dir.y};
      if (input.IsInside(newIndex) && input.Get(newIndex) == '#')
      {
        Index2D newDir{-dir.y, dir.x};
        dir = newDir;
      }
      else
      {
        index = newIndex;
      }
    }
    return visited.size();
  }

  OUTPUT2(input)
  {
    std::set<Index2D> visited;
    Index2D index = input.Find('^');
    Index2D dir{0, -1};
    int32_t sum = 0;
    while (input.IsInside(index))
    {
      Index2D newIndex{index.x + dir.x, index.y + dir.y};
      if (input.IsInside(newIndex) && input.Get(newIndex) == '#')
      {
        Index2D newDir{-dir.y, dir.x};
        dir = newDir;
      }
      else
      {
        if (visited.count(newIndex) == 0)
        {
          if (TestObstacle(input, newIndex, index, dir))
            sum++;
        }
        visited.emplace(index);
        index = newIndex;
      }
    }

    return sum;
  }
}
