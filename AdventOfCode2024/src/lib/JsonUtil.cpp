#include "JsonUtil.h"

#include "Json.h"

#include <cstring>
#include <utility>


bool JsonUtil::IsWhiteSpace(char c)
{
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

JsonUtil::CursorPos JsonUtil::GetStringPos(const std::string& str, uint64_t pos)
{
  CursorPos cursor;
  for (uint64_t i = 0; i < pos; i++)
  {
    if (str[i] == '\n')
    {
      cursor.x = 1;
      cursor.y++;
    }
    else
    {
      cursor.x++;
    }
  }
  return cursor;
}

bool JsonUtil::ReadWhiteSpace(const std::string& str, uint64_t& pos)
{
  while (pos < str.size() && IsWhiteSpace(str[pos]))
  {
    pos++;
  }
  return pos < str.size();
}

bool JsonUtil::IsNumber(char c)
{
  return c >= '0' && c <= '9';
}

bool JsonUtil::IsFloat(const std::string& str, uint64_t pos)
{
  if (str[pos] == '-')
    pos++;
  while (pos < str.size() && IsNumber(str[pos]))
  {
    pos++;
  }
  if (pos >= str.size())
    return false;
  if (str[pos] != '.')
    return false;
  return true;
}

bool JsonUtil::IsNumber(const std::string& str, uint64_t pos)
{
  if (str[pos] == '-')
    pos++;
  while (pos < str.size() && IsNumber(str[pos]))
  {
    pos++;
  }
  if (pos >= str.size())
    return false;
  return true;
}

bool JsonUtil::IsKeyword(const std::string& str, uint64_t& pos, const std::string& s)
{
  if (std::strncmp(str.c_str() + pos, s.c_str(), s.size()) != 0)
    return false;
  pos += s.size();
  return true;
}

// "\\"  is currently not possible, need to fix that at some point
// or any double escaped \\ before a "
bool JsonUtil::ReadString(const std::string& str, uint64_t& pos, Json& json)
{
  size_t min = 10 < str.size() - pos ? 10 : str.size() - pos;
  std::string_view view{str.c_str() + pos, min};
  if (pos >= str.size())
  {
    std::cout << "EOF at start of string" << std::endl;
    return false;
  }
  if (str[pos] != '\"')
  {
    std::cout << GetStringPos(str, pos) << ": String does not start with citation: " << view << std::endl;
    return false;
  }
  pos++;
  uint64_t stringStart = pos;
  while (true)
  {
    while (pos < str.size() && str[pos] != '\"') pos++;
    if (pos >= str.size() || str[pos - 1] != '\\') break;
  }

  if (pos >= str.size())
  {
    pos = stringStart - 1;
    std::cout << GetStringPos(str, pos) << ": String did not close with citation: " << view << std::endl;
    return false;
  }
  pos++;
  json.value.emplace<std::string>(str.begin() + stringStart, str.begin() + pos - 1);
  return true;
}
