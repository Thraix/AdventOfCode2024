#pragma once

#include <map>

template<typename T, typename S>
struct Mapper
{
  std::map<T, S> map1;
  std::map<S, T> map2;

  Mapper()
  {}

  Mapper(const std::map<T, S>& map)
    : map1{map}
  {
    for (auto&& elem : map)
    {
      auto&& res = map2.emplace({elem.second, elem.first});
      if (!res.first)
      {
        map1.clear();
        map2.clear();
        std::cout << "Mapper : mappings are not unique : " << elem.second << std::endl;
        return;
      }
    }
  }

  Mapper(const std::initializer_list<std::pair<T, S>>& map)
  {
    for (auto&& elem : map)
    {
      auto&& res = map1.emplace(elem.first, elem.second);
      if (!res.second)
      {
        map1.clear();
        map2.clear();
        std::cout << "Mapper : mappings are not unique : " << elem.first << std::endl;
        return;
      }
      auto&& res2 = map2.emplace(elem.second, elem.first);
      if (!res2.second)
      {
        map1.clear();
        map2.clear();
        std::cout << "Mapper : mappings are not unique : " << elem.second << std::endl;
        return;
      }
    }
  }

  void Emplace(const T& t, const S& s)
  {
    map1.emplace(t, s);
    map2.emplace(s, t);
  }

  T* GetFirst(const S& key)
  {
    auto it = map2.find(key);
    if(it == map2.end())
      return nullptr;
    return &it->second;
  }

  S* GetSecond(const T& key)
  {
    auto it = map1.find(key);
    if(it == map1.end())
      return nullptr;
    return &it->second;
  }
};
