#include "aoc.h"

namespace day15
{
  struct Map
  {
    Array2D<char> map;
    std::string input;
  };

  REGISTER_DAY(day15, Map, int64_t);

  REGISTER_TEST_EXAMPLE(day15, ExampleInput, 1, 10092);
  REGISTER_TEST        (day15, Input,        1, 1526673);
  REGISTER_TEST_EXAMPLE(day15, ExampleInput, 2, 9021);
  REGISTER_TEST        (day15, Input,        2, 1535509);

  READ_INPUT(input)
  {
    Map map;
    map.map = Input::ReadArray2D(input);

    std::string str;
    while (std::getline(input, str))
      map.input += str;

    return map;
  }

  void Move(Array2D<char>& map, Index2D boxPos, Index2D dir)
  {
    if (dir.x == 0)
    {
      char leftC = map.Get(boxPos.x, boxPos.y + dir.y);
      if (leftC == '[')
        Move(map, Index2D{boxPos.x, boxPos.y + dir.y}, dir);
      if (leftC == ']')
        Move(map, Index2D{boxPos.x - 1, boxPos.y + dir.y}, dir);

      char rightC = map.Get(boxPos.x + 1, boxPos.y + dir.y);
      if (rightC == '[')
        Move(map, Index2D{boxPos.x + 1, boxPos.y + dir.y}, dir);
      if (rightC == ']')
        Move(map, Index2D{boxPos.x, boxPos.y + dir.y}, dir);

      map.Set(boxPos.x, boxPos.y + dir.y, '[');
      map.Set(boxPos.x + 1, boxPos.y + dir.y, ']');
      map.Set(boxPos.x, boxPos.y, '.');
      map.Set(boxPos.x + 1, boxPos.y, '.');
    }
    else
    {
      if (map.Get(boxPos.x + dir.x * 2, boxPos.y) == '[')
        Move(map, Index2D{boxPos.x + dir.x * 2, boxPos.y}, dir);
      map.Set(boxPos.x + dir.x, boxPos.y, '[');
      map.Set(boxPos.x + dir.x + 1, boxPos.y, ']');
    }
  }

  bool CanMove(const Array2D<char>& map, Index2D boxPos, Index2D dir);

  bool CanMoveSide(const Array2D<char>& map, Index2D boxSidePos, Index2D dir)
  {
    char c = map[boxSidePos];
    if (c == '.')
      return true;
    else if (c == '[')
      return CanMove(map, Index2D{boxSidePos.x, boxSidePos.y}, dir);
    else if (c == ']')
      return CanMove(map, Index2D{boxSidePos.x - 1, boxSidePos.y}, dir);
    return false;
  }

  // boxPos is position of [
  bool CanMove(const Array2D<char>& map, Index2D boxPos, Index2D dir)
  {
    if (dir.x == 0)
    {
      bool canMoveLeftSide = CanMoveSide(map, Index2D{boxPos.x, boxPos.y + dir.y}, dir);
      bool canMoveRightSide = CanMoveSide(map, Index2D{boxPos.x + 1, boxPos.y + dir.y}, dir);
      return canMoveLeftSide && canMoveRightSide;
    }
    else
    {
      Index2D pos = boxPos + dir;
      while (true)
      {
        char n = map[pos];

        if (n == '.')
          return true;
        else if (n == '#')
          return false;

        pos = pos + dir;
      }
    }
  }

  int64_t CalculateResult(const Array2D<char>& map)
  {
    int64_t sum = 0;
    for (auto it = map.begin(); it != map.end(); it++)
      if (*it == 'O' || *it == '[')
        sum += it.index.x + it.index.y * 100;

    return sum;
  }

  OUTPUT1(input)
  {
    Map cpy = input;
    Index2D position = cpy.map.Find('@');
    for (auto& dirC : cpy.input)
    {
      Index2D dir = Helper::GetDirection(dirC);

      char nextChar = cpy.map[position + dir];
      if (nextChar == '.')
      {
        cpy.map[position] = '.';
        cpy.map[position + dir] = '@';
        position = position + dir;
      }
      else if (nextChar == 'O')
      {
        Index2D pos = position + dir;
        while (true)
        {
          char n = cpy.map[pos];
          if (n == '.')
          {
            cpy.map[position] = '.';
            cpy.map[position + dir] = '@';
            cpy.map[pos] = 'O';
            position = position + dir;
            break;
          }
          else if (n == '#')
            break;
          pos = pos + dir;
        }
      }
    }

    return CalculateResult(cpy.map);
  }

  OUTPUT2(input)
  {
    Array2D<char> map{input.map.width * 2, input.map.height, '.'};
    for (auto it = input.map.begin(); it != input.map.end(); it++)
    {
      if (*it == 'O')
      {
        map.Set(it.index.x * 2, it.index.y, '[');
        map.Set(it.index.x * 2 + 1, it.index.y, ']');
      }
      else if (*it == '#')
      {
        map.Set(it.index.x * 2, it.index.y, '#');
        map.Set(it.index.x * 2 + 1, it.index.y, '#');
      }
      else if (*it == '@')
      {
        map.Set(it.index.x * 2, it.index.y, '@');
      }
    }

    Index2D position = map.Find('@');
    for (auto& dirC : input.input)
    {
      Index2D dir = Helper::GetDirection(dirC);

      char nextChar = map[position + dir];
      if (nextChar == '.')
      {
        map[position] = '.';
        map[position + dir] = '@';
        position = position + dir;
      }
      else if (nextChar == ']' || nextChar == '[')
      {
        Index2D boxPos = position + dir;
        if (nextChar == ']')
          boxPos.x -= 1;
        if (CanMove(map, boxPos, dir))
        {
          Move(map, boxPos, dir);
          map[position] = '.';
          map[position + dir] = '@';
          position = position + dir;
        }
      }
    }

    return CalculateResult(map);
  }
}
