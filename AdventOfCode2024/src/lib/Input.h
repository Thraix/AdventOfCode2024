#pragma once

#include "Array2D.h"

#include <stdint.h>
#include <iostream>
#include <string>

// Makes it possible to parse strings directly without having to put it in a variable
// Ie std::cin >> "number: " >> number;
static std::istream& operator>>(std::istream& stream, char const* pattern) {
  char c;
  while (stream.peek() == ' ')
    stream.get(c);

  while(*pattern != '\0' && stream && *pattern == stream.peek() && stream.get(c))
    ++pattern;

  if (*pattern != '\0')
    stream.setstate(std::ios::failbit);

  return stream;
}

struct Input
{
  static Array2D<char> ReadArray2D(std::istream& input)
  {
    std::vector<char> data;
    int width = 0;
    int height = 0;
    std::string str;
    while (getline(input, str))
    {
      if (str.empty())
        break;
      width = (int)str.length();
      height++;
      for (auto c : str)
        data.emplace_back(c);
    }
    return Array2D<char>(width, height, data);
  }

  static Array2D<int> ReadDigitsAsArray2D(std::istream& input)
  {
    std::vector<int> data;
    int width = 0;
    int height = 0;
    std::string str;
    while (getline(input, str))
    {
      width = (int)str.length();
      height++;
      for (auto c : str)
        data.emplace_back(c - '0');
    }
    return Array2D<int>(width, height, data);
  }


  static std::vector<int> ReadInts(std::istream& input)
  {
    return ReadInts(input, '\n');
  }

  static std::vector<int64_t> ReadInt64s(std::istream& input)
  {
    return ReadInt64s(input, '\n');
  }

  static std::vector<int> ReadInts(std::istream& input, char delimiter)
  {
    std::vector<int> ints;
    std::string str;
    while (getline(input, str, delimiter))
      ints.emplace_back(std::atoi(str.c_str()));
    return ints;
  }

  static std::vector<int64_t> ReadInt64s(std::istream& input, char delimiter)
  {
    std::vector<int64_t> ints;
    std::string str;
    while (getline(input, str, delimiter))
      ints.emplace_back(std::atoll(str.c_str()));
    return ints;
  }

  static std::vector<std::string> ReadLines(std::istream& input)
  {
    std::vector<std::string> strings;
    std::string str;
    while (getline(input, str))
      strings.emplace_back(str);
    return strings;
  }
};
