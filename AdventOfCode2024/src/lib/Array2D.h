#pragma once

#include <iostream>
#include <cassert>
#include <vector>

struct Index2D
{
  int x = -1;
  int y = -1;

  bool IsValid()
  {
    return x != -1 && y != -1;
  }

  operator bool()
  {
    return IsValid();
  }

  Index2D operator+(const Index2D& other) const
  {
    return Index2D{x + other.x, y + other.y};
  }

  Index2D operator-(const Index2D& other) const
  {
    return Index2D{x - other.x, y - other.y};
  }

  Index2D operator*(int scale) const
  {
    return Index2D{x * scale, y * scale};
  }

  friend std::ostream& operator<<(std::ostream& stream, const Index2D& index)
  {
    return stream << "(" << index.x << " " << index.y << ")";
  }

  friend bool operator<(const Index2D& lhs, const Index2D& rhs)
  {
    if(lhs.x != rhs.x)
      return lhs.x < rhs.x;
    return lhs.y < rhs.y;
  }

  friend bool operator==(const Index2D& lhs, const Index2D& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  friend bool operator!=(const Index2D& lhs, const Index2D& rhs)
  {
    return !operator==(lhs, rhs);
  }
};


template <typename T>
struct Array2D
{
  struct ConstIterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = const T;
    using pointer           = const T*;  // or also value_type*
    using reference         = const T&;  // or also value_type&

    const Array2D<T>& array2D;
    Index2D index;

    ConstIterator(const Array2D<T>& array2D, const Index2D& index)
      : array2D{array2D}, index{index}
    {}

    reference operator*() const { return array2D.Get(index); }
    pointer operator->() const { return &array2D.Get(index); }

    // Prefix
    ConstIterator& operator++() 
    {
      index.x++;
      if (index.x == array2D.width)
      {
        index.x = 0;
        index.y++;
      }
      return *this;
    }

    // Postfix
    ConstIterator operator++(int)
    {
      ConstIterator tmp = *this; ++(*this); return tmp;
    }

    bool operator==(const ConstIterator& rhs)
    {
      return index == rhs.index;
    }

    bool operator!=(const ConstIterator& rhs)
    {
      return !(*this == rhs);
    }
  };
  struct Iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;  // or also value_type*
    using reference         = T&;  // or also value_type&

    Array2D<T>& array2D;
    Index2D index;

    Iterator(Array2D<T>& array2D, const Index2D& index)
      : array2D{array2D}, index{index}
    {}

    reference operator*() const { return array2D.Get(index); }
    pointer operator->() const { return &array2D.Get(index); }

    // Prefix
    Iterator& operator++() 
    {
      index.x++;
      if (index.x == array2D.width)
      {
        index.x = 0;
        index.y++;
      }
      return *this;
    }

    // Postfix
    Iterator operator++(int) 
    {
      Iterator tmp = *this; ++(*this); return tmp;
    }

    bool operator==(const Iterator& rhs)
    {
      return index == rhs.index;
    }

    bool operator!=(const Iterator& rhs)
    {
      return !(*this == rhs);
    }
  };

  int width;
  int height;
  std::vector<T> array2D;

  Array2D()
    : width{0}, height{0}
  {}

  Array2D(int width, int height)
    : width{width}, height{height}
  {
    array2D.resize(width * height);
  }

  Array2D(int width, int height, const T& fill)
    : width{width}, height{height}
  {
    array2D = std::vector<T>(width * height, fill);
    array2D.resize(width * height);
  }

  Array2D(int width, int height, const std::vector<T>& data)
    : width{width}, height{height}, array2D{data}
  {}

  void Fill(const T& value)
  {
    for (int i = 0; i < width * height; i++)
    {
      array2D[i] = value;
    }
  }

  int GetIndex(int x, int y) const
  {
    assert(IsInside(x, y));
    return x + y * width;
  }

  T& Get(int x, int y) 
  {
    assert(IsInside(x, y));
    return array2D[x + y * width];
  }

  const T& Get(int x, int y) const
  {
    assert(IsInside(x, y));
    return array2D[x + y * width];
  }

  T& Get(const Index2D& index) 
  {
    return Get(index.x, index.y);
  }

  const T& Get(const Index2D& index) const
  {
    return Get(index.x, index.y);
  }

  T& operator[](const Index2D& index)
  {
    return Get(index.x, index.y);
  }

  const T& operator[](const Index2D& index) const
  {
    return Get(index.x, index.y);
  }

  T& GetMirror(int x, int y) 
  {
    // if width = 3, it will repeat x like this: 012210012210, same with height
    x = (x / width ) % 2 == 0 ? x % width  : width  - (x % width ) - 1;
    y = (y / height) % 2 == 0 ? y % height : height - (y % height) - 1;
    return array2D[x + y * width];
  }

  const T& GetMirror(int x, int y) const
  {
    // if width = 3, it will repeat x like this: 012210012210, same with height
    x = (x / width ) % 2 == 0 ? x % width  : width  - (x % width ) - 1;
    y = (y / height) % 2 == 0 ? y % height : height - (y % height) - 1;
    return array2D[x + y * width];
  }

  T& GetMod(int x, int y) 
  {
    return array2D[((x % width + width) % width) + ((y % height + height) % height) * width];
  }

  const T& GetMod(int x, int y) const
  {
    return array2D[((x % width + width) % width) + ((y % height + height) % height) * width];
  }

  void Set(int x, int y, const T& val)
  {
    Get(x, y) = val;
  }

  void SetMod(int x, int y, const T& val)
  {
    GetMod(x, y) = val;
  }

  void SetMirror(int x, int y, const T& val)
  {
    GetMirror(x, y) = val;
  }

  int GetNeighbors(const T& val, int x, int y, int size = 1) const
  {
    int neighbors = 0;
    for (int yi = y - size; yi <= y + size; yi++)
    {
      for (int xi = x - size; xi <= x + size; xi++)
      {
        if (xi < 0 || yi < 0 || xi >= width || yi >= height)
          continue;
        if (xi == x && yi == y)
          continue;
        if (Get(xi, yi) == val)
          neighbors++;
      }
    }
    return neighbors;
  }

  std::vector<Index2D> GetNeighbors(const Index2D& index, bool includeDiagonals) const
  {
    std::vector<Index2D> neighbors;
    if (index.x > 0)
      neighbors.emplace_back(Index2D{index.x - 1, index.y});
    if (index.y > 0)
      neighbors.emplace_back(Index2D{index.x, index.y - 1});
    if (index.x < width - 1)
      neighbors.emplace_back(Index2D{index.x + 1, index.y});
    if (index.y < height - 1)
      neighbors.emplace_back(Index2D{index.x, index.y + 1});
    if (includeDiagonals)
    {
			if (index.x > 0 && index.y > 0)
				neighbors.emplace_back(Index2D{index.x - 1, index.y - 1});
			if (index.x > 0 && index.y < height - 1)
				neighbors.emplace_back(Index2D{index.x - 1, index.y + 1});
			if (index.x < width - 1 && index.y < height - 1)
				neighbors.emplace_back(Index2D{index.x + 1, index.y + 1});
			if (index.x < width - 1 && index.y > 0)
				neighbors.emplace_back(Index2D{index.x + 1, index.y - 1});
    }
    return neighbors;
  }

  bool IsInside(int x, int y) const
  {
    return x >= 0 && y >= 0 && x < width && y < height;
  }

  bool IsInside(const Index2D& index) const
  {
    return IsInside(index.x, index.y);
  }

  int Count(const T& val) const
  {
    int count = 0;
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        if (Get(x, y) == val)
          count++;
      }
    }
    return count;
  }

  void RotateCW()
  {
    Array2D cpy = *this;
    for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
      {
        array2D[height - y - 1 + x * height] = cpy.Get(x, y);
      }
    }
    std::swap(width, height);
  }

  void RotateCCW()
  {
    Array2D cpy = *this;
    for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
      {
        array2D[y + (width - x - 1) * height] = cpy.Get(x, y);
      }
    }
    std::swap(width, height);
  }

  void Rotate180()
  {
    Array2D cpy = *this;
    for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
      {
        array2D[width - x - 1 + (height - y - 1) * width] = cpy.Get(x, y);
      }
    }
  }

  void FlipVertical()
  {
    for(int y = 0; y < height / 2; y++)
    {
      for(int x = 0; x < width; x++)
      {
        std::swap(array2D[GetIndex(x, y)], array2D[GetIndex(x, height-y-1)]);
      }
    }
  }

  void FlipHorizontal()
  {
    for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width / 2; x++)
      {
        std::swap(array2D[GetIndex(x, y)], array2D[GetIndex(width - x - 1, y)]);
      }
    }
  }

  template <typename Function>
  void Each(Function func) const
  {
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        func(*this, Index2D{x, y});
      }
    }
  }

  Index2D Find(const T& value) const
  {
    for(int y = 0; y < height; y++)
    {
      for(int x = 0; x < width; x++)
      {
        if(Get(x, y) == value)
          return Index2D{x, y};
      }
    }
    return Index2D{};
  }

  void Increment(Index2D& index) const
  {
    index.x++;
    if(index.x != width)
      return;
    index.x = 0;
    index.y++;
  }

  typename Array2D<T>::Iterator begin()
  {
    return Iterator{*this, Index2D{0, 0}};
  }

  typename Array2D<T>::Iterator end()
  {
    return Iterator{*this, Index2D{0, height}};
  }

  typename Array2D<T>::ConstIterator begin() const
  {
    return ConstIterator{*this, Index2D{0, 0}};
  }

  typename Array2D<T>::ConstIterator end() const
  {
    return ConstIterator{*this, Index2D{0, height}};
  }

  friend std::ostream& operator<<(std::ostream& os, const Array2D& array2D)
  {
    for (int y = 0; y < array2D.height; y++) 
    {
      for (int x = 0; x < array2D.width; x++) 
      {
        os << array2D.Get(x, y);
      }
      os << std::endl;
    }
    return std::cout;
  }
};
