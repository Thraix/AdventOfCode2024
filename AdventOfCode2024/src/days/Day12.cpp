#include "aoc.h"

namespace day12
{
  REGISTER_DAY(day12, Array2D<char>, int);

  REGISTER_TEST_EXAMPLE(day12, ExampleInput, 1, 1930);
  REGISTER_TEST        (day12, Input,        1, 1550156);
  REGISTER_TEST_EXAMPLE(day12, ExampleInput, 2, 1206);
  REGISTER_TEST        (day12, Input,        2, 946084);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  void CalculateAreaAndPerimiter(const Array2D<char>& map, Index2D position, std::set<Index2D>& totalVisited, std::set<Index2D>& area, std::set<std::pair<Index2D, Index2D>>& perimeter)
  {
    const std::vector<Index2D> indices{Index2D{-1, 0}, Index2D{1, 0}, Index2D{0, -1}, Index2D{0, 1}};

    char c = map.Get(position);
    std::stack<Index2D> toEvaluate;
    toEvaluate.emplace(position);
    area.emplace(position);
    while (!toEvaluate.empty())
    {
      Index2D current = toEvaluate.top();
      toEvaluate.pop();
      totalVisited.emplace(current);

      for (auto& dir : indices)
      {
        Index2D newPos = current + dir;
        if (area.count(newPos) == 0)
        {
          if (map.IsInside(newPos) && map.Get(newPos) == c)
          {
            toEvaluate.emplace(newPos);
            area.emplace(newPos);
          }
          else
            perimeter.emplace(current, dir);
        }
      }
    }
  }

  void RemovePerimetersNextToPos(std::set<std::pair<Index2D, Index2D>>& perimeter, Index2D pos, Index2D normal, Index2D dir)
  {
    Index2D next = pos + dir;
    auto it = perimeter.find({next, normal});
    while (it != perimeter.end())
    {
      perimeter.erase(it);
      next = next + dir;
      it = perimeter.find({next, normal});
    }
  }

  int CalculatePerimeterSideSize(std::set<std::pair<Index2D, Index2D>>& perimeter)
  {
    int perimeterSize = 0;
    while(!perimeter.empty())
    {
      auto [index, dir] = *perimeter.begin();
      perimeter.erase(perimeter.begin());
      perimeterSize++;

      RemovePerimetersNextToPos(perimeter, index, dir, Index2D{dir.y, dir.x});
      RemovePerimetersNextToPos(perimeter, index, dir, Index2D{-dir.y, -dir.x});
    }
    return perimeterSize;
  }


  OUTPUT1(input)
  {
    int sum = 0;
    std::set<Index2D> visited;
    for (auto it = input.begin(); it != input.end(); it++)
    {
      Index2D index = it.index;
      if (visited.count(index) == 0)
      {
        std::set<Index2D> area;
        std::set<std::pair<Index2D, Index2D>> perimeter;
        CalculateAreaAndPerimiter(input, it.index, visited, area, perimeter);

        sum += perimeter.size() * area.size();
      }
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int sum = 0;
    std::set<Index2D> visited;
    for (auto it = input.begin(); it != input.end(); it++)
    {
      if (visited.count(it.index) == 0)
      {
        std::set<Index2D> area;
        std::set<std::pair<Index2D, Index2D>> perimeter;
        CalculateAreaAndPerimiter(input, it.index, visited, area, perimeter);
        int perimeterSize = CalculatePerimeterSideSize(perimeter);

        sum += perimeterSize * area.size();
      }
    }
    return sum;
  }
}
