#include "aoc.h"

namespace day18
{
  using Memory = std::map<Index2D, int>;
  REGISTER_DAY(day18, Memory, int32_t);

  REGISTER_TEST_EXAMPLE(day18, ExampleInput, 1, 22);
  REGISTER_TEST        (day18, Input,        1, 304);
  REGISTER_TEST_EXAMPLE(day18, ExampleInput, 2, 20);
  REGISTER_TEST        (day18, Input,        2, 2876);

  READ_INPUT(input)
  {
    std::map<Index2D, int> memory;
    std::string str;
    int i = 0;
    while (std::getline(input, str))
    {
      Index2D index;
      std::stringstream  ss{str};
      ss >> index.x >> "," >> index.y;
      memory.emplace(index, i);
      i++;
    }
    return memory;
  }

  struct Map
  {
    const std::map<Index2D, int>& memory;
    int size;
    int time;
  };

  std::vector<std::pair<int, Index2D>> Branch(const Map& map, const Index2D& current)
  {
    std::vector<std::pair<int, Index2D>> branch;
    for (auto& dir : Helper::GetNeighborDirections())
    {
      Index2D pos = current + dir;
      if (pos.x < 0 || pos.y < 0 || pos.x > map.size || pos.y > map.size)
        continue;
      auto it = map.memory.find(pos);
      if (it == map.memory.end() || it->second >= map.time)
        branch.emplace_back(1, pos);
    }
    return branch;
  }

  bool Goal(const Map& map, const Index2D& current)
  {
    return current.x == map.size && current.y == map.size;
  }

  bool ValidPath(const Map& map, int index)
  {
    Map cpy = map;
    cpy.time = index;
    return Helper::Dijkstras(cpy, Index2D{0, 0}, Branch, Goal) != 0;
  }

  OUTPUT1(input)
  {
    int32_t sum = 0;
    int time = 1024;
    int size = 70;
    if (isExample)
    {
      size = 6;
      time = 12;
    }
    Map map{input, size, time};
    return Helper::Dijkstras(map, Index2D{0, 0}, Branch, Goal);
  }

  OUTPUT2(input)
  {
    int size = 70;
    if (isExample)
      size = 6;

    Map map{input, size, 0};
    int result = Helper::BinarySearch(map, 0, input.size(), ValidPath);
    for (auto& index : input)
    {
      if (index.second == result)
      {
        std::cout << index.first.x << "," << index.first.y << std::endl;
        return result;
      }
    }

    return 0;
  }
}
