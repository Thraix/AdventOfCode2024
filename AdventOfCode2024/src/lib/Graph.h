#pragma once

#include <map>
#include <set>

template <typename Key, typename Value>
class Graph
{
  std::map<Key, std::map<Key, Value>> graph;

  // Data based on the graph
  std::set<Key> nodes;

public:
  Graph() = default;

  void AddNode(const Key& from)
  {
    graph.emplace(from);
  }

  void AddOneWayEdge(const Key& from, const Key& to, const Value& value)
  {
    graph[from][to] = value;
    nodes.emplace(from);
  }

  void AddTwoWayEdge(const Key& from, const Key& to, const Value& value)
  {
    graph[from][to] = value;
    graph[to][from] = value;
    nodes.emplace(from);
    nodes.emplace(to);
  }

  Value DFSLong(const Key& from, const Key& to)
  {
    std::set<Key> visited;
    return DFSLong(to, visited, from, Value{});
  }

  Value DFS(const Key& from, const Key& to)
  {
    std::set<Key> visited;
    return DFS(to, visited, from, Value{});
  }

  Value Dijkstras(const Key& from, const Key& to) const
  {
    std::map<Key, Value> visited;
    std::multimap<Value, Key> openSet;
    openSet.emplace(Value{}, from);
    while (!openSet.empty())
    {
      const auto& [cost, node] = *openSet.begin();

      if (node == to)
        return cost;

      for (auto& [next, nextCost] : graph.find(node)->second)
      {
        auto it = visited.find(next);
        if (it == visited.end() || it->second > cost + nextCost)
        {
          openSet.emplace(cost + nextCost, next);
          visited[next] = cost + nextCost;
        }
      }
      openSet.erase(openSet.begin());
    }
    return Value{};
  }

  std::pair<Value, std::vector<Key>> DijkstrasWithPath(const Key& from, const Key& to) const
  {
    std::map<Key, Value> visited;
    std::multimap<Value, std::vector<Key>> openSet;
    openSet.emplace(Value{}, std::vector<Key>{from});
    while (!openSet.empty())
    {
      const auto& [cost, path] = *openSet.begin();

      if (path.back() == to)
        return {cost, path};

      for (auto& [next, nextCost] : graph.find(path.back())->second)
      {
        auto it = visited.find(next);
        if (it == visited.end() || it->second > cost + nextCost)
        {
          std::vector<Key> newPath = path;
          newPath.emplace_back(next);
          openSet.emplace(cost + nextCost, newPath);
          visited[next] = cost + nextCost;
        }
      }
      openSet.erase(openSet.begin());
    }
    return {Value{}, {}};
  }

  const Value& GetEdge(const Key& from, const Key& to) const
  {
    // Verification that the nodes exists in the graph?
    return graph.find(from)->second.find(to)->second;
  }

  const std::map<Key, Value>& GetPaths(const Key& from) const
  {
    return graph.find(from)->second;
  }

  const std::set<Key>& GetNodes() const
  {
    return nodes;
  }

private:
  Value DFSLong(const Key& to, std::set<Key>& visited, const Key& current, const Value& cost)
  {
    if (visited.count(current))
      return Value{};
    if (current == to)
      return cost;

    visited.emplace(current);
    Value max{};
    for (auto& [next, nextCost] : graph.find(current)->second)
    {
      max = std::max(max, DFSLong(to, visited, next, cost + nextCost));
    }
    visited.erase(current);
    return max;
  }

  Value DFS(const Key& to, std::set<Key>& visited, const Key& current, const Value& cost)
  {
    if (visited.count(current))
      return Value{};
    if (current == to)
      return cost;

    visited.emplace(current);
    for (auto& [next, nextCost] : graph.find(current)->second)
    {
      Value val = DFS(to, visited, next, cost + nextCost);
      if (val != Value{})
        return val;
    }
    visited.erase(current);
    return Value{};
  }
};