#include "aoc.h"

namespace day21
{
  REGISTER_DAY(day21, std::vector<std::string>, int64_t);

  REGISTER_TEST_EXAMPLE(day21, ExampleInput, 1, 126384);
  REGISTER_TEST(day21, Input, 1, 177814);
  // REGISTER_TEST_EXAMPLE(day21, ExampleInput, 2, 0);
  REGISTER_TEST(day21, Input, 2, 0);

  READ_INPUT(input)
  {
    return Input::ReadLines(input);
  }

  Index2D CharToCoord(char c)
  {
    if (c == '7')
      return Index2D{0, 0};
    if (c == '8')
      return Index2D{1, 0};
    if (c == '9')
      return Index2D{2, 0};

    if (c == '4')
      return Index2D{0, 1};
    if (c == '5')
      return Index2D{1, 1};
    if (c == '6')
      return Index2D{2, 1};

    if (c == '1')
      return Index2D{0, 2};
    if (c == '2')
      return Index2D{1, 2};
    if (c == '3')
      return Index2D{2, 2};

    if (c == '0')
      return Index2D{1, 3};
    if (c == 'A')
      return Index2D{2, 3};
  }

  bool SameDir(Index2D prev, Index2D cur, Index2D next)
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

  std::vector<std::vector<Index2D>> possiblePaths(Index2D from, Index2D to, bool keypad)
  {
    if (keypad)
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
        for (auto dir : Helper::GetNeighborDirections())
        {
          Index2D newPos = path.back() + dir;
          if (visited.count(newPos) == 0 && newPos != Index2D{0, 3} && newPos.x >= 0 && newPos.x <= 2 && newPos.y >= 0 && newPos.y <= 3)
          {
            std::vector<Index2D> newPath = path;
            if (path.size() == 1)
              newPath.emplace_back(newPos);
            else if (!SameDir(path[path.size() - 2], path.back(), newPos))
              newPath.emplace_back(newPos);
            else
              newPath.back() = newPos;
            openPath.emplace(newPath);
          }
        }
      }
      return paths;
    }
    else
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
        for (auto dir : Helper::GetNeighborDirections())
        {
          Index2D newPos = path.back() + dir;
          if (visited.count(newPos) == 0 && newPos != Index2D{0, 0} && newPos.x >= 0 && newPos.x <= 2 && newPos.y >= 0 && newPos.y <= 1)
          {
            std::vector<Index2D> newPath = path;
            if (path.size() == 1)
              newPath.emplace_back(newPos);
            else if (!SameDir(path[path.size() - 2], path.back(), newPos))
              newPath.emplace_back(newPos);
            else
              newPath.back() = newPos;
            openPath.emplace(newPath);
          }
        }
      }
      return paths;
    }
  }

  Index2D MoveCoord(Index2D from, Index2D to)
  {
    if (from.x < to.x)
      return Index2D{2, 1};
    if (from.x > to.x)
      return Index2D{0, 1};
    if (from.y < to.y)
      return Index2D{1, 1};
    if (from.y > to.y)
      return Index2D{1, 0};

    assert(false);
  }

  struct State
  {
    Index2D from;
    Index2D to;
    int depth;
    int amount;

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

  int64_t Move(int maxDepth, Index2D from, Index2D to, int depth, int amount, std::map<State, int64_t>& memoization)
  {
    if (depth == maxDepth)
    {
      Index2D diff = to - from;
#if 0
      for (int i = 0; i < diff.x; i++)
        std::cout << '>';
      for (int i = 0; i < -diff.x; i++)
        std::cout << '<';
      for (int i = 0; i < diff.y; i++)
        std::cout << 'v';
      for (int i = 0; i < -diff.y; i++)
        std::cout << '^';
      for(int i = 0; i < amount; i++)
        std::cout << 'A';
#endif
      return std::abs(diff.x) + std::abs(diff.y) + amount;
    }
    State state{from ,to, depth, amount};
    if (memoization.find(state) != memoization.end())
      return memoization[state];

    std::vector<std::vector<Index2D>> paths = possiblePaths(from, to, depth == 0);
    // std::cout << depth << ": " << paths << std::endl;
    int64_t min = std::numeric_limits<int64_t>::max();
    int iMin = 0;
    for (int i = 0; i < paths.size(); i++)
    {
      Index2D moveFrom = Index2D{2, 0};
      int64_t sum = 0;
      for (int j = 0; j < paths[i].size() - 1; j++)
      {
        Index2D moveTo = MoveCoord(paths[i][j], paths[i][j + 1]);

        int diff = std::abs(paths[i][j].x - paths[i][j + 1].x + paths[i][j].y - paths[i][j + 1].y);
        sum += Move(maxDepth, moveFrom, moveTo, depth + 1, diff, memoization);

        moveFrom = moveTo;
      }

      int diff = std::abs(paths[i].back().x - paths[i][0].x + paths[i].back().y - paths[i][0].y);
      sum += Move(maxDepth, moveFrom, Index2D{2, 0}, depth + 1, amount, memoization);

      min = std::min(sum, min);
      if (sum == min)
        iMin = i;
    }
    memoization[State{from ,to, depth, amount}] = min;
    // std::cout << "BestPath: " << from << " " << to << " " << depth << " " << paths[iMin] << " " << min << std::endl;
    return min;

#if 0

    int64_t sum = 0;
    if (diff.x < 0 && diff.y < 0)
    {
#if 1
      int64_t s = 0;
      s += Move(Index2D{2, 0}, Index2D{1, 0}, depth + 1, std::abs(diff.y));
      s += Move(Index2D{1, 0}, Index2D{0, 1}, depth + 1, std::abs(diff.x));
      s += Move(Index2D{0, 1}, Index2D{2, 0}, depth + 1, amount);
#endif

#if 0 // illegal
      int64_t s2 = 0;
      s2 += Move(Index2D{2, 0}, Index2D{0, 1}, depth + 1, std::abs(diff.x));
      s2 += Move(Index2D{0, 1}, Index2D{1, 0}, depth + 1, std::abs(diff.y));
      s2 += Move(Index2D{1, 0}, Index2D{2, 0}, depth + 1, amount);
#endif

#if 0
      int64_t s3 = 0;
      s3 += Move(Index2D{2, 0}, Index2D{1, 0}, depth + 1, std::abs(diff.y));
      s3 += Move(Index2D{1, 0}, Index2D{0, 1}, depth + 1, std::abs(diff.x));
      s3 += Move(Index2D{0, 1}, Index2D{2, 0}, depth + 1, amount);
#endif
      sum += s; // std::min(s, s2);

      // std::cout << "1: " << s << " " << s2 << std::endl;
    }
    else if (diff.x < 0 && diff.y > 0)
    {
#if 0
      int64_t s = 0;
      s += Move(Index2D{2, 0}, Index2D{1, 1}, depth + 1, std::abs(diff.y));
      s += Move(Index2D{1, 1}, Index2D{0, 1}, depth + 1, std::abs(diff.x));
      s += Move(Index2D{0, 1}, Index2D{2, 0}, depth + 1, amount);
#endif

      int64_t s2 = 0;
      s2 += Move(Index2D{2, 0}, Index2D{0, 1}, depth + 1, std::abs(diff.x));
      s2 += Move(Index2D{0, 1}, Index2D{1, 1}, depth + 1, std::abs(diff.y));
      s2 += Move(Index2D{1, 1}, Index2D{2, 0}, depth + 1, amount);

      sum += s2;// std::min(s, s2);
      // std::cout << "2: " << s << " " << s2 << std::endl;
    }
    else if (diff.x > 0 && diff.y > 0)
    {
#if 0
      int64_t s = 0;
      s += Move(Index2D{2, 0}, Index2D{2, 1}, depth + 1, std::abs(diff.x));
      s += Move(Index2D{2, 1}, Index2D{1, 1}, depth + 1, std::abs(diff.y));
      s += Move(Index2D{1, 1}, Index2D{2, 0}, depth + 1, amount);
#endif

      int64_t s2 = 0;
      s2 += Move(Index2D{2, 0}, Index2D{1, 1}, depth + 1, std::abs(diff.y));
      s2 += Move(Index2D{1, 1}, Index2D{2, 1}, depth + 1, std::abs(diff.x));
      s2 += Move(Index2D{2, 1}, Index2D{2, 0}, depth + 1, amount);

      sum += s2;// std::min(s, s2);
      // std::cout << "3: " << s << " " << s2 << std::endl;
    }
    else if (diff.x > 0 && diff.y < 0)
    {
#if 0
      int64_t s = 0;
      s += Move(Index2D{2, 0}, Index2D{1, 0}, depth + 1, std::abs(diff.y));
      s += Move(Index2D{1, 0}, Index2D{2, 1}, depth + 1, std::abs(diff.x));
      s += Move(Index2D{2, 1}, Index2D{2, 0}, depth + 1, amount);
#endif

      int64_t s2 = 0;
      s2 += Move(Index2D{2, 0}, Index2D{2, 1}, depth + 1, std::abs(diff.x));
      s2 += Move(Index2D{2, 1}, Index2D{1, 0}, depth + 1, std::abs(diff.y));
      s2 += Move(Index2D{1, 0}, Index2D{2, 0}, depth + 1, amount);

      sum += s2;// std::min(s, s2);
      // std::cout << "4: " << s << " " << s2 << std::endl;
    }
    else if (diff.x < 0)
    {
      sum += Move(Index2D{2, 0}, Index2D{0, 1}, depth + 1, std::abs(diff.x));
      sum += Move(Index2D{0, 1}, Index2D{2, 0}, depth + 1, amount);
    }
    else if (diff.x > 0)
    {
      sum += Move(Index2D{2, 0}, Index2D{2, 1}, depth + 1, std::abs(diff.x));
      sum += Move(Index2D{2, 1}, Index2D{2, 0}, depth + 1, amount);
    }
    else if (diff.y > 0)
    {
      sum += Move(Index2D{2, 0}, Index2D{1, 1}, depth + 1, std::abs(diff.y));
      sum += Move(Index2D{1, 1}, Index2D{2, 0}, depth + 1, amount);
    }
    else if (diff.y < 0)
    {
      sum += Move(Index2D{2, 0}, Index2D{1, 0}, depth + 1, std::abs(diff.y));
      sum += Move(Index2D{1, 0}, Index2D{2, 0}, depth + 1, amount);
    }

    return sum;
#endif
  }

  char PrintKeyPad(Index2D pos)
  {
    if (pos.x == 0 && pos.y == 0)
      return '7';
    if (pos.x == 1 && pos.y == 0)
      return '8';
    if (pos.x == 2 && pos.y == 0)
      return '9';
    if (pos.x == 0 && pos.y == 1)
      return '4';
    if (pos.x == 1 && pos.y == 1)
      return '5';
    if (pos.x == 2 && pos.y == 1)
      return '6';
    if (pos.x == 0 && pos.y == 2)
      return '1';
    if (pos.x == 1 && pos.y == 2)
      return '2';
    if (pos.x == 2 && pos.y == 2)
      return '3';
    if (pos.x == 1 && pos.y == 3)
      return '0';
    if (pos.x == 2 && pos.y == 3)
      return 'A';
    return ' ';
  }

  char PrintMovePad(Index2D pos)
  {
    if (pos.x == 1 && pos.y == 0)
      return '^';
    if (pos.x == 2 && pos.y == 0)
      return 'A';
    if (pos.x == 0 && pos.y == 1)
      return '<';
    if (pos.x == 1 && pos.y == 1)
      return 'v';
    if (pos.x == 2 && pos.y == 1)
      return '>';
    return ' ';
  }

  void PrintRobotPositions(const std::string& input)
  {
    std::vector<Index2D> robotPos{3, Index2D{2, 0}};
    robotPos.back() = Index2D{2, 3};
    int i = 0;
    for (char c : input)
    {
      std::cout << c << std::endl;
      i++;
      if (c != 'A')
      {
        Index2D offset = Helper::GetDirection(c);
        robotPos[0] = robotPos[0] + offset;
      }
      else
      {
        if (PrintMovePad(robotPos[0]) != 'A')
        {
          Index2D offset = Helper::GetDirection(PrintMovePad(robotPos[0]));
          robotPos[1] = robotPos[1] + offset;
        }
        else
        {
          if (PrintMovePad(robotPos[1]) != 'A')
          {
            Index2D offset = Helper::GetDirection(PrintMovePad(robotPos[1]));
            robotPos[2] = robotPos[2] + offset;
          }
          else
          {
            std::cout << i << " " << PrintKeyPad(robotPos.back()) << std::endl;
          }
        }
      }
#if 1
      for (int i = 0; i < robotPos.size() - 1; i++)
      {
        std::cout << PrintMovePad(robotPos[i]);
        std::cout << " ";
      }
      std::cout << PrintKeyPad(robotPos.back());
        std::cout << std::endl;
        std::cout << std::endl;
#endif
    }
  }

  int64_t Solve(const std::vector<std::string>& input, int depth)
  {
    int64_t sum = 0;
    for (int i = 0; i < input.size(); i++)
    {
      std::cout << input[i] << std::endl;
      Index2D pos = CharToCoord('A');
      int64_t s = 0;
      for (auto c : input[i])
      {
        std::map<State, int64_t> memoization;
        int64_t move = Move(depth, pos, CharToCoord(c), 0, 1, memoization);
        s += move;
        std::cout << " " << move << " " << CharToCoord(c) << " " << pos << " " << s << std::endl;
        pos = CharToCoord(c);
      }
      int num = std::stoi(input[i]);
      sum += num * s;
      std::cout << std::endl;
      std::cout << s << std::endl;
    }

    return sum;
  }



  OUTPUT1(input)
  {
    std::cout << possiblePaths(Index2D{0, 0}, Index2D{2, 3}, true) << std::endl;
    PrintRobotPositions("<<vAA>A>^AAvA<^A>AvA^A");

    std::cout << "----------------------" << std::endl;

    PrintRobotPositions("<v<A>>^AAAvA^A<vA<AA>>^AvAA<^A>A<v<A>A>^AAAvA<^A>A<vA>^A<A>A");
    return Solve(input, 2);
  }

  OUTPUT2(input)
  {
    return Solve(input, 25);
  }
}

//
