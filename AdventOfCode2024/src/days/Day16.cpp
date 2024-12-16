#include "aoc.h"

namespace day16
{
  REGISTER_DAY(day16, Array2D<char>, int32_t);

  REGISTER_TEST_EXAMPLE(day16, ExampleInput, 1, 7036);
  REGISTER_TEST        (day16, Input,        1, 72428);
  REGISTER_TEST_EXAMPLE(day16, ExampleInput, 2, 45);
  REGISTER_TEST        (day16, Input,        2, 456);

  READ_INPUT(input)
  {
    return Input::ReadArray2D(input);
  }

  std::vector<std::pair<int, std::pair<Index2D, Index2D>>> Branch(const Array2D<char>& map, const std::pair<Index2D, Index2D>& state)
  {
    std::vector<std::pair<int, std::pair<Index2D, Index2D>>> paths;
    if (map.Get(state.first + state.second) != '#')
      paths.emplace_back(1, std::pair{state.first + state.second, state.second});

    Index2D dir2 = Index2D{state.second.y, -state.second.x};
    if (map.Get(state.first + dir2) != '#')
      paths.emplace_back(1001, std::pair{state.first + dir2, dir2});

    Index2D dir3 = Index2D{-state.second.y, state.second.x};
    if (map.Get(state.first + dir3) != '#')
      paths.emplace_back(1001, std::pair{state.first + dir3, dir3});

    return paths;
  }

  bool Goal(const Array2D<char>& map, const std::pair<Index2D, Index2D>& state)
  {
    return map.Get(state.first) == 'E';
  }

  OUTPUT1(input)
  {
    return Helper::Dijkstras(input, std::pair{input.Find('S'), Index2D{1, 0}}, Branch, Goal);
  }

  OUTPUT2(input)
  {
    std::set<std::pair<Index2D, Index2D>> allVisited = Helper::DijkstrasAllVisited(input, std::pair{input.Find('S'), Index2D{1, 0}}, Branch, Goal);

    std::set<Index2D> solutions;
    for (const auto& [pos, dir] : allVisited)
      solutions.emplace(pos);

    return solutions.size();
  }
}
