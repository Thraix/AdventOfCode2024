#include "aoc.h"

namespace day09
{
  REGISTER_DAY(day09, std::vector<int>, int64_t);

  REGISTER_TEST_EXAMPLE(day09, ExampleInput, 1, 1928);
  REGISTER_TEST        (day09, Input,        1, 6283404590840);
  REGISTER_TEST_EXAMPLE(day09, ExampleInput, 2, 2858);
  REGISTER_TEST        (day09, Input,        2, 6304576012713);

  READ_INPUT(input)
  {
    std::string str;
    std::getline(input, str);
    if (str.size() % 2 == 0)
      str.pop_back();
    std::vector<int> disk;
    std::transform(str.begin(), str.end(), std::back_inserter(disk), [](char c) { return c - '0'; });
    return disk;
  }

  struct File
  {
    int64_t pos;
    int64_t size;
    int64_t id;
  };

  OUTPUT1(input)
  {
    int64_t sum = 0;
    std::vector<std::pair<int, int>> resMemory;
    resMemory.emplace_back(input[0], 0);
    int currentFreeIndex = 1;
    int currentFreeMemory = input[currentFreeIndex];
    int currentMemoryIndex = input.size() - 1;
    int currentMemoryToMove = input[currentMemoryIndex];
    while (true)
    {
      if (currentMemoryToMove <= currentFreeMemory)
      {
        resMemory.emplace_back(currentMemoryToMove, currentMemoryIndex / 2);
        currentFreeMemory -= currentMemoryToMove;
        currentMemoryIndex -= 2;
        currentMemoryToMove = input[currentMemoryIndex];
        if (currentMemoryIndex < currentFreeIndex)
          break;
      }
      else
      {
        resMemory.emplace_back(currentFreeMemory, currentMemoryIndex / 2);
        currentMemoryToMove -= currentFreeMemory;
        currentFreeMemory = 0;
      }

      if (currentFreeMemory == 0)
      {
        if (currentFreeIndex + 1 == currentMemoryIndex)
        {
          resMemory.back().first += currentMemoryToMove;
          break;
        }
        else
        {
          resMemory.emplace_back(input[currentFreeIndex + 1], (currentFreeIndex + 1) / 2);
          currentFreeIndex += 2;
          currentFreeMemory = input[currentFreeIndex];
        }
      }
    }

    int64_t pos = 0;
    for (auto [count, id] : resMemory)
    {
      // Based on sum of integer formula:
      // S = numberOfIntegers * (firstNumber + lastNumber) / 2
      int64_t firstNumber = pos;
      int64_t lastNumber = pos + count - 1;
      sum += count * (firstNumber + lastNumber) / 2 * id;
      pos += count;
    }
    return sum;
  }

  OUTPUT2(input)
  {
    int64_t sum = 0;
    std::map<int64_t, int64_t> idToPos;
    std::vector<File> resMemory;
    std::vector<std::pair<int64_t, int64_t>> freeMemory;
    int pos = 0;
    for (int i = 0; i < input.size(); i += 2)
    {
      idToPos.emplace(i / 2, pos);
      pos += input[i];
      if (i != input.size() - 1)
      {
        freeMemory.emplace_back(pos, input[i + 1]);
        pos += input[i + 1];
      }
    }

    for (int i = input.size() - 1; i >= 0; i -= 2)
    {
      int fileSize = input[i];
      int fileId = i / 2;
      int filePos = idToPos[fileId];

      for (int j = 0; j < freeMemory.size(); j++)
      {
        auto& [freeMemoryPos, freeMemorySize] = freeMemory[j];
        if (freeMemoryPos > filePos)
        {
          // Couldn't find a suitable place for the file, don't move it
          resMemory.emplace_back(File{filePos, fileSize, fileId});
          break;
        }

        if (freeMemorySize >= fileSize)
        {
          resMemory.emplace_back(File{freeMemoryPos, fileSize, fileId});
          freeMemorySize -= fileSize;
          freeMemoryPos += fileSize;
          if (freeMemorySize == 0)
            freeMemory.erase(freeMemory.begin() + j);
          break;
        }
      }
    }

    for (const auto& file : resMemory)
    {
      // Based on sum of integer formula:
      // S = numberOfIntegers * (firstNumber + lastNumber) / 2
      int64_t firstNumber = file.pos;
      int64_t lastNumber = file.pos + file.size - 1;
      sum += file.size * (firstNumber + lastNumber) / 2 * file.id;
    }

    return sum;
  }
}
