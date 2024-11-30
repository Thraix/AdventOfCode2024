#pragma once

#include <iostream>
#include <string>
#include <stdint.h>

class Json;

class JsonUtil
{
private:
  static bool IsWhiteSpace(char c);

public:
  struct CursorPos
  {
    uint64_t x = 1;
    uint64_t y = 1;
    friend std::ostream& operator<<(std::ostream& os, const CursorPos& pos)
    {
      return os << "(" << pos.y << "," << pos.x << ")";
    }
  };

  static CursorPos GetStringPos(const std::string& str, uint64_t pos);

  static bool ReadWhiteSpace(const std::string& str, uint64_t& pos);

  static bool IsNumber(char c);

  static bool IsFloat(const std::string& str, uint64_t pos);

  static bool IsNumber(const std::string& str, uint64_t pos);

  static bool IsKeyword(const std::string& str, uint64_t& pos, const std::string& s);

  // "\\"  is currently not possible, need to fix that at some point
  // or any double escaped \\ before a "
  static bool ReadString(const std::string& str, uint64_t& pos, Json& json);
};