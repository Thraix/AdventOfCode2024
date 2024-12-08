#include "aoc.h"

namespace day08
{
  struct Data
  {
    std::map<char, std::vector<Index2D>> map;
    int width;
    int height;
  };
  REGISTER_DAY(day08, Data, int32_t);

  REGISTER_TEST_EXAMPLE(day08, ExampleInput, 1, 14);
  REGISTER_TEST        (day08, Input,        1, 244);
  REGISTER_TEST_EXAMPLE(day08, ExampleInput, 2, 34);
  REGISTER_TEST        (day08, Input,        2, 912);

  READ_INPUT(input)
  {
    Data data;
    std::string str;
    int y = 0;
    while (std::getline(input, str))
    {
      for (int i = 0; i < str.size(); i++) 
      {
        if (str[i] != '.')
          data.map[str[i]].emplace_back(Index2D{i, y});
      }
      data.width = str.size();
      y++;
    }
    data.height = y;
    return data;
  }

  OUTPUT1(input)
  {
    std::set<Index2D> antinodes;
    int32_t sum = 0;
    for (auto& [freq, locations] : input.map)
    {
      for (int i = 0; i < locations.size(); i++)
      {
        for (int j = i + 1; j < locations.size(); j++)
        {
          Index2D diff = locations[i] - locations[j];
          Index2D newPos1 = locations[i] + diff;
          Index2D newPos2 = locations[j] - diff;
          if (newPos1.x >= 0 && newPos1.x < input.width && newPos1.y >= 0 && newPos1.y < input.height)
            antinodes.emplace(newPos1);
          if (newPos2.x >= 0 && newPos2.x < input.width && newPos2.y >= 0 && newPos2.y < input.height)
            antinodes.emplace(newPos2);
        }
      }
    }
    return antinodes.size();
  }

  OUTPUT2(input)
  {
    std::set<Index2D> antinodes;
    for (auto& [freq, locations] : input.map)
    {
      for (int i = 0; i < locations.size(); i++)
      {
        for (int j = i + 1; j < locations.size(); j++)
        {
          Index2D diff = locations[i] - locations[j];
          int offset = 0;
          while (true)
          {
            Index2D newPos = locations[i] + diff * offset;
            if (newPos.x < 0 || newPos.x >= input.width || newPos.y < 0 || newPos.y >= input.height)
              break;
            antinodes.emplace(newPos);
            offset++;
          }
          offset = 0;
          while (true)
          {
            Index2D newPos = locations[j] - diff * offset;
            if (newPos.x < 0 || newPos.x >= input.width || newPos.y < 0 || newPos.y >= input.height)
              break;
            antinodes.emplace(newPos);
            offset++;
          }
        }
      }
    }
    return antinodes.size();
  }
}
