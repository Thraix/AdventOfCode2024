#include "aoc.h"

namespace day20
{
  REGISTER_DAY(day20, Array2D<char>, int32_t);

  REGISTER_TEST_EXAMPLE(day20, ExampleInput, 1, 1);
  REGISTER_TEST        (day20, Input,        1, 1429);
  REGISTER_TEST_EXAMPLE(day20, ExampleInput, 2, 285);
  REGISTER_TEST        (day20, Input,        2, 988931);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  // Single path DFS, assumes the path can never hit a dead end
  static std::pair<int, std::vector<Index2D>> DFS(const Array2D<char>& map, const Index2D& initial)
  {
    std::vector<Index2D> path;
    Index2D prev = Index2D{-1, -1};
    std::pair<int, Index2D> cur{0, initial};
    path.emplace_back(initial);

    while (map[cur.second] != 'E')
    {
      for (auto dir : Helper::GetNeighborDirections())
      {
        Index2D pos = cur.second + dir;
        if (map.IsInside(pos))
        {
          if (pos != prev && map[pos] != '#')
          {
            prev = cur.second;
            cur.first++;
            cur.second = pos;
            path.emplace_back(pos);
            break;
          }
        }
      }
    }
    return {cur.first, path};
  }

  int Cheat(const Array2D<char>& map, int cheatTime, int diff)
  {
    int sum = 0;
    std::pair<int, std::vector<Index2D>> path = DFS(map, map.Find('S'));

    std::vector<int> distanceToGoal(map.width * map.height, -1);
    for (int i = 0; i < path.second.size(); i++)
    {
      distanceToGoal[map.GetIndex(path.second[i])] = path.second.size() - i - 1;
    }

    for (int i = 0; i < path.second.size(); i++)
    {
      for (int y = -cheatTime; y <= cheatTime; y++)
      {
        int yAbs = std::abs(y);
        for (int x = -cheatTime + yAbs; x <= cheatTime - yAbs; x++)
        {
          int xAbs = std::abs(x);
          int cost = i + xAbs + yAbs;
          Index2D pos{path.second[i].x + x, path.second[i].y + y};
          if (map.IsInside(pos))
          {
            int index = map.GetIndex(pos);
            if (distanceToGoal[index] >= 0)
            {
              if (cost + distanceToGoal[index] <= path.first - diff)
              {
                sum++;
              }
            }
          }
        }
      }
    }
    return sum;
  }

  OUTPUT1(input)
  {
    return Cheat(input, 2, isExample ? 50 : 100);
  }

  OUTPUT2(input)
  {
    return Cheat(input, 20, isExample ? 50 : 100);
  }
}
