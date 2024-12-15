#include "aoc.h"

namespace day14
{
  struct Robot
  {
    Index2D pos;
    Index2D vel;
  };

  REGISTER_DAY(day14, std::vector<Robot>, int32_t);

  REGISTER_TEST_EXAMPLE(day14, ExampleInput, 1, 12);
  REGISTER_TEST(day14, Input, 1, 228421332);
  // REGISTER_TEST_EXAMPLE(day14, ExampleInput, 2, 31);
  REGISTER_TEST(day14, Input, 2, 7790);

  READ_INPUT(input)
  {
    std::vector<Robot> robots;
    std::string str;
    while (std::getline(input, str))
    {
      Robot robot;
      std::stringstream ss{str};
      ss >> "p=" >> robot.pos.x >> "," >> robot.pos.y >> "v=" >> robot.vel.x >> "," >> robot.vel.y;
      robots.emplace_back(robot);
    }
    return robots;
  }

  OUTPUT1(input)
  {
    int width = 101;
    int height = 103;
    if (isExample)
    {
      width = 11;
      height = 7;
    }

    int a{0}, b{0}, c{0}, d{0};
    for (auto& robot : input)
    {
      Index2D endPos = robot.pos + robot.vel * 100;
      endPos.x = (endPos.x % width + width) % width;
      endPos.y = (endPos.y % height + height) % height;
      if (endPos.x < width / 2 && endPos.y < height / 2)
        a++;
      if (endPos.x < width / 2 && endPos.y > height / 2)
        b++;
      if (endPos.x > width / 2 && endPos.y < height / 2)
        c++;
      if (endPos.x > width / 2 && endPos.y > height / 2)
        d++;
    }

    return a * b * c * d;
  }

  OUTPUT2(input)
  {
    int width = 101;
    int height = 103;
    std::vector<Robot> cpy = input;
    int i = 0;
    int64_t maxNeighbors = 0;
    int minSteps = 0;
    while (i < width * height)
    {
      i++;

      std::set<Index2D> positions;
      for (auto& robot : cpy)
      {
        robot.pos = robot.pos + robot.vel;
        robot.pos.x = (robot.pos.x + width) % width;
        robot.pos.y = (robot.pos.y + height) % height;
        positions.emplace(robot.pos);
      }

      int64_t neighbors = 0;
      for (const auto& pos : positions)
      {
        for (const auto& dir : Helper::GetNeighborDirections())
        {
          neighbors += positions.count(pos + dir);
        }
      }

      if (neighbors > maxNeighbors)
      {
        maxNeighbors = neighbors;
        minSteps = i;
      }
    }

    return minSteps;
  }
}
