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

  int GetStepCoord(const std::vector<Robot>& robots, int length, int dimension)
  {
    int maxIndex = 0;
    int maxNeighbors = 0;
    for (int i = 1; i <= length; i++)
    {
      std::vector<int> amounts(length, 0);
      for (const auto& robot : robots)
      {
        int endPos = robot.pos[dimension] + robot.vel[dimension] * i;
        endPos = (endPos % length + length) % length;
        amounts[endPos]++;
      }

      int neighbors = 0;
      for (int i = 0; i < length; i++)
      {
        neighbors += amounts[i] * (amounts[(i + 1) % length] + amounts[(i + length - 1) % length]);
      }
      if (neighbors > maxNeighbors)
      {
        maxNeighbors = neighbors;
        maxIndex = i;
      }
    }
    return maxIndex;
  }

  OUTPUT2(input)
  {
    int width = 101;
    int height = 103;
    int stepX = GetStepCoord(input, width, 0);
    int stepY = GetStepCoord(input, height, 1);

    return Helper::ChineseRemainderTheorem({width, height}, {stepX, stepY});
  }
}
