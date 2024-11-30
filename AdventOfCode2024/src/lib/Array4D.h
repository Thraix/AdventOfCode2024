#pragma once

#include <iostream>
#include <vector>

struct Index4D
{
  int x = -1;
  int y = -1;
  int z = -1;
  int w = -1;

  bool IsValid()
  {
    return x != -1 && y != -1 && z != -1 && w != -1;
  }

  operator bool()
  {
    return IsValid();
  }

  Index4D operator+(const Index4D& other) const
  {
    return Index4D{x + other.x, y + other.y, z + other.z, w + other.w};
  }

  Index4D operator-(const Index4D& other) const
  {
    return Index4D{x - other.x, y - other.y, z - other.z, w - other.w};
  }

  friend std::ostream& operator<<(std::ostream& stream, const Index4D& index)
  {
    return stream << "(" << index.x << " " << index.y << " " << index.z << " " << index.w << ")";
  }

  friend bool operator<(const Index4D& lhs, const Index4D& rhs)
  {
    if(lhs.x != rhs.x)
      return lhs.x < rhs.x;
    if(lhs.y != rhs.y)
      return lhs.y < rhs.y;
    if(lhs.z != rhs.z)
      return lhs.z < rhs.z;
    return lhs.w < rhs.w;
  }

  friend bool operator==(const Index4D& lhs, const Index4D& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
  }

  friend bool operator!=(const Index4D& lhs, const Index4D& rhs)
  {
    return !operator==(lhs, rhs);
  }
};

template <typename T>
struct Array4D
{
  int width;
  int height;
  int length;
  int time;
  std::vector<T> array4D;

  Array4D()
    : width{0}, height{0}, length{0}, time{0}
  {}

  Array4D(int width, int height, int length, int time)
    : width{width}, height{height}, length{length}, time{time}
  {
    array4D.resize(width * height * length * time);
  }

  Array4D(int width, int height, int length, int time, const std::vector<T>& data)
    : width{width}, height{height}, length{length}, time{time}, array4D{data}
  {}

  void Fill(const T& value)
  {
    for (int i = 0; i < width * height * length * time; i++)
    {
      array4D[i] = value;
    }
  }

  int GetIndex(int x, int y, int z, int w) const
  {
    return x + y * width + z * width * height + w * width * height * length;
  }

  T& Get(int x, int y, int z, int w) 
  {
    return array4D[x + y * width + z * width * height + w * width * height * length];
  }

  const T& Get(int x, int y, int z, int w) const
  {
    return array4D[x + y * width + z * width * height + w * width * height * length];
  }

  T& operator[](const Index4D& index)
  {
    return Get(index.x, index.y, index.z, index.w);
  }

  const T& operator[](const Index4D& index) const
  {
    return Get(index.x, index.y, index.z, index.w);
  }

  T& GetMirror(int x, int y, int z, int w) 
  {
    // if width = 3, it will repeat x like this: 012210012210, same with height and length
    x = (x / width ) % 2 == 0 ? x % width  : width  - (x % width ) - 1;
    y = (y / height) % 2 == 0 ? y % height : height - (y % height) - 1;
    z = (z / length) % 2 == 0 ? z % length : length - (z % length) - 1;
    w = (w / time  ) % 2 == 0 ? w % time   : time   - (z % time  ) - 1;
    return array4D[x + y * width + z * width * height + w * width * height * length];
  }

  const T& GetMirror(int x, int y, int z, int w) const
  {
    // if width = 3, it will repeat x like this: 012210012210, same with height and length
    x = (x / width ) % 2 == 0 ? x % width  : width  - (x % width ) - 1;
    y = (y / height) % 2 == 0 ? y % height : height - (y % height) - 1;
    z = (z / length) % 2 == 0 ? z % length : length - (z % length) - 1;
    w = (w / time  ) % 2 == 0 ? w % time   : time   - (z % time  ) - 1;
    return array4D[x + y * width + z * width * height + w * width * height * length];
  }

  T& GetMod(int x, int y, int z, int w) 
  {
    return array4D[((x % width  + width ) % width) + 
                   ((y % height + height) % height) * width + 
                   ((z % length + length) % length) * width * height + 
                   ((w % time   + time  ) % time  ) * width * height * length];
  }

  const T& GetMod(int x, int y, int z, int w) const
  {
    return array4D[((x % width  + width ) % width) + 
                   ((y % height + height) % height) * width + 
                   ((z % length + length) % length) * width * height + 
                   ((w % time   + time  ) % time  ) * width * height * length];
  }

  void Set(int x, int y, int z, int w, const T& val)
  {
    Get(x, y, z, w) = val;
  }

  void SetMod(int x, int y, int z, int w, const T& val)
  {
    GetMod(x, y, z, w) = val;
  }

  void SetMirror(int x, int y, int z, int w, const T& val)
  {
    GetMirror(x, y, z, w) = val;
  }

  int GetNeighbors(const T& val, int x, int y, int z, int w, int size = 1) const
  {
    int neighbors = 0;
    for (int wi = w - size; wi <= w + size; wi++)
    {
      for (int zi = z - size; zi <= z + size; zi++)
      {
        for (int yi = y - size; yi <= y + size; yi++)
        {
          for (int xi = x - size; xi <= x + size; xi++)
          {
            if (xi < 0 || yi < 0 || zi < 0 || wi < 0 || xi >= width || yi >= height || zi >= length || wi >= time)
              continue;
            if (xi == x && yi == y && zi == z && wi == w)
              continue;
            if (Get(xi, yi, zi, wi) == val)
              neighbors++;
          }
        }
      }
    }
    return neighbors;
  }

  bool IsInside(const Index4D& index) const
  {
    return index.x >= 0 && index.y >= 0 && index.z >= 0 && index.w >= 0 && index.x < width && index.y < height && index.z < length && index.w < time;
  }

  int Count(const T& val)
  {
    int count = 0;
    for (int w = 0; w < length; w++)
    {
      for (int z = 0; z < length; z++)
      {
        for (int y = 0; y < height; y++)
        {
          for (int x = 0; x < width; x++)
          {
            if (Get(x, y, z, w) == val)
              count++;
          }
        }
      }
    }
    return count;
  }

  template <typename Function>
  void Each(T func) const
  {
    for (int w = 0; w < length; w++)
    {
      for (int z = 0; z < length; z++)
      {
        for (int y = 0; y < height; y++)
        {
          for (int x = 0; x < width; x++)
          {
            func(*this, Index4D{x, y, z, w});
          }
        }
      }
    }
  }

  Index4D Find(const T& value)
  {
    for(int w = 0; w < time; w++)
    {
      for(int z = 0; z < length; z++)
      {
        for(int y = 0; y < height; y++)
        {
          for(int x = 0; x < width; x++)
          {
            if(Get(x, y, z, w) == value)
              return Index4D{x, y, z, w};
          }
        }
      }
    }
    return Index4D{};
  }

  void Increment(Index4D& index)
  {
    index.x++;
    if(index.x != width)
      return;
    index.x = 0;
    index.y++;
    if(index.y != height)
      return;
    index.y = 0;
    index.z++;
    if(index.z != length)
      return;
    index.z = 0;
    index.w++;
  }

  typename std::vector<T>::iterator begin()
  {
    return array4D.begin();
  }

  typename std::vector<T>::iterator end()
  {
    return array4D.end();
  }

  typename std::vector<T>::const_iterator begin() const
  {
    return array4D.cbegin();
  }

  typename std::vector<T>::const_iterator end() const
  {
    return array4D.cend();
  }

  friend std::ostream& operator<<(std::ostream& os, const Array4D& array4D)
  {
    for (int w = 0; w < array4D.time; w++)
    {
      for (int z = 0; z < array4D.length; z++)
      {
        std::cout << "w=" << w << " z=" << z << std::endl;
        for (int y = 0; y < array4D.height; y++)
        {
          for (int x = 0; x < array4D.width; x++)
          {
            std::cout << array4D.Get(x, y, z, w);
          }
          std::cout << std::endl;
        }
      }
    }
    return std::cout;
  }
};
