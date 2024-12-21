#include "aoc.h"

namespace day21
{
  REGISTER_DAY(day21, std::vector<std::string>, int64_t);

  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 1, 126384);
  REGISTER_TEST(day21, Input, 1, 177814);
  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 2, 154115708116294);
  REGISTER_TEST(day21, Input, 2, 220493992841852);

  READ_INPUT(input)
  {
    return Input::ReadLines(input);
  }

  Index2D KeyPadCoord(char c)
  {
    if (c == '7') return Index2D{0, 0};
    if (c == '8') return Index2D{1, 0};
    if (c == '9') return Index2D{2, 0};

    if (c == '4') return Index2D{0, 1};
    if (c == '5') return Index2D{1, 1};
    if (c == '6') return Index2D{2, 1};

    if (c == '1') return Index2D{0, 2};
    if (c == '2') return Index2D{1, 2};
    if (c == '3') return Index2D{2, 2};

    if (c == '0') return Index2D{1, 3};
    if (c == 'A') return Index2D{2, 3};

    abort();
  }

  Index2D MovePadCoord(const Index2D& from, const Index2D& to)
  {
    if (from.x < to.x) return Index2D{2, 1};
    if (from.x > to.x) return Index2D{0, 1};
    if (from.y < to.y) return Index2D{1, 1};
    if (from.y > to.y) return Index2D{1, 0};

    abort();
  }

  bool IsSameDir(const Index2D& prev, const Index2D& cur, const Index2D& next)
  {
    Index2D diffA = cur - prev;
    Index2D diffB = next - cur;

    if (diffA.x < 0 && diffB.x < 0)
      return true;
    if (diffA.x > 0 && diffB.x > 0)
      return true;
    if (diffA.y < 0 && diffB.y < 0)
      return true;
    if (diffA.y > 0 && diffB.y > 0)
      return true;

    return  false;
  }

  std::vector<std::vector<Index2D>> PossiblePaths(const Index2D& from, const Index2D& to, bool keypad)
  {
    std::set<Index2D> visited;
    std::queue<std::vector<Index2D>> openPath;
    openPath.emplace(std::vector<Index2D>{from});
    std::vector<std::vector<Index2D>> paths;

    while (!openPath.empty())
    {
      auto path = openPath.front();
      openPath.pop();
      visited.emplace(path.back());

      if (path.back() == to)
      {
        paths.emplace_back(path);
        continue;
      }

      for (const auto& dir : Helper::GetNeighborDirections())
      {
        Index2D newPos = path.back() + dir;
        bool keyPadCondition = keypad && newPos != Index2D{0, 3} && newPos.x >= 0 && newPos.x <= 2 && newPos.y >= 0 && newPos.y <= 3;
        bool movePadCondition = !keypad && newPos != Index2D{0, 0} && newPos.x >= 0 && newPos.x <= 2 && newPos.y >= 0 && newPos.y <= 1;
        if (visited.count(newPos) == 0 && (keyPadCondition || movePadCondition))
        {
          std::vector<Index2D> newPath = path;

          if (path.size() == 1)
            newPath.emplace_back(newPos);
          else if (!IsSameDir(path[path.size() - 2], path.back(), newPos))
            newPath.emplace_back(newPos);
          else
            newPath.back() = newPos;

          openPath.emplace(newPath);
        }
      }
    }
    return paths;
  }

  struct State
  {
    Index2D from;
    Index2D to;
    int depth;
    int64_t amount;

    bool operator<(const State& other) const
    {
      if (from != other.from)
        return from < other.from;
      if (to != other.to)
        return to < other.to;
      if (depth != other.depth)
        return depth < other.depth;
      return amount < other.amount;
    }
  };

  int64_t Move(int maxDepth, const Index2D& from, const Index2D& to, int depth, int64_t amount, std::map<State, int64_t>& memoization)
  {
    if (depth == maxDepth)
      return Helper::ManhattanDistance(to, from) + amount;

    State state{from ,to, depth, amount};
    auto it = memoization.find(state);
    if (it != memoization.end())
      return it->second;

    std::vector<std::vector<Index2D>> paths = PossiblePaths(from, to, depth == 0);
    int64_t min = std::numeric_limits<int64_t>::max();
    for (const auto& path : paths)
    {
      Index2D moveFrom = Index2D{2, 0};
      int64_t sum = 0;
      for (int j = 0; j < path.size() - 1; j++)
      {
        Index2D moveTo = MovePadCoord(path[j], path[j + 1]);

        int diff = Helper::ManhattanDistance(path[j], path[j + 1]);
        sum += Move(maxDepth, moveFrom, moveTo, depth + 1, diff, memoization);

        moveFrom = moveTo;
      }

      int diff = Helper::ManhattanDistance(path.back(), path.front());
      sum += Move(maxDepth, moveFrom, Index2D{2, 0}, depth + 1, amount, memoization);

      min = std::min(sum, min);
    }

    memoization[state] = min;
    return min;
  }

  int64_t Solve(const std::vector<std::string>& input, int depth)
  {
    std::map<State, int64_t> memoization;
    int64_t sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      Index2D moveFrom = KeyPadCoord('A');
      int64_t codeSum = 0;
      for (auto c : input[i])
      {
        Index2D keyPadCoord = KeyPadCoord(c);
        codeSum += Move(depth, moveFrom, keyPadCoord, 0, 1, memoization);
        moveFrom = keyPadCoord;
      }
      int64_t codeNum = std::stoi(input[i]);
      sum += codeNum * codeSum;
    }

    return sum;
  }

  OUTPUT1(input)
  {
    return Solve(input, 2);
  }

  OUTPUT2(input)
  {
    return Solve(input, 25);
  }
}
