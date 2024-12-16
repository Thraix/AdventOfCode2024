#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <vector>

#define BIT(x) (1 << (x))

struct Helper
{
  template <typename T, typename S>
  static void RemoveIf(T& t, S func)
  {
    t.erase(std::remove_if(t.begin(), t.end(), func), t.end());
  }

  static bool StartsWith(const std::string_view& str, const std::string& prefix)
  {
    return str.substr(0, prefix.size()) == prefix;
  }

  static bool EndsWith(const std::string_view& str, const std::string& prefix)
  {
    return str.substr(str.size() - prefix.size(), prefix.size()) == prefix;
  }

  static bool IsDigit(char c)
  {
    return c >= '0' && c <= '9';
  }

  // Converts a binary string to an int32_t ie "10100111001" to 1337
  static int BinStrToInt(const std::string& str)
  {
    if (str.size() >= 32)
    {
      std::cout << "BinStrToInt: Too big string, use BinStrToInt64 instead" << std::endl;
      return 0;
    }

    int val = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
      if (str[i] == '1')
        val |= (1 << (str.size() - i - 1));
    }
    return val;
  }

  // Converts a binary string to an int64_t ie "10100111001" to 1337
  static int64_t BinStrToInt64(const std::string& str)
  {
    if (str.size() >= 64)
    {
      std::cout << "BinStrToInt64: Too big string" << std::endl;
      return 0;
    }

    int64_t val = 0;
    for (size_t i = 0; i < str.size(); i++)
    {
      if (str[i] == '1')
        val |= (1ll << (str.size() - i - 1));
    }
    return val;
  }

  template <typename T, typename S>
  static T Sum(const S& container)
  {
    return std::accumulate(container.begin(), container.end(), (T)0, std::plus<T>());
  }

  template <typename T, typename S, typename Container>
  static T Sum(const Container& container, S func)
  {
    return std::accumulate(container.begin(), container.end(), (T)0, func);
  }

  template <typename T, typename S>
  static T Product(const S& container)
  {
    return std::accumulate(container.begin(), container.end(), (T)1, std::multiplies<T>());
  }

  template <typename T, typename S, typename Container>
  static T Product(const Container& container, S func)
  {
    return std::accumulate(container.begin(), container.end(), (T)1, func);
  }

  template <typename Container>
  static typename Container::value_type Min(const Container& container)
  {
    if (container.size() == 0)
    {
      std::cout << "Min: Container empty" << std::endl;
      return typename Container::value_type{};
    }
    typename Container::value_type min = *container.begin();
    for (auto&& element : container)
    {
      min = std::min(min, element);
    }
    return min;
  }

  template <typename Container>
  static typename Container::value_type Max(const Container& container)
  {
    if (container.size() == 0)
    {
      std::cout << "Min: Container empty" << std::endl;
      return typename Container::value_type{};
    }
    typename Container::value_type max = *container.begin();
    for (auto&& element : container)
    {
      max = std::max(max, element);
    }
    return max;
  }

  template <typename T>
  static T Sign(const T& t)
  {
    return t > 0 ? 1 : t < 0 ? -1 : 0;
  }

  template <typename T>
  static std::set<T> GetUnion(const std::vector<std::set<T>>& list)
  {
    std::set<char> uniq;
    for (auto l : list)
    {
      for (auto t : l)
      {
        uniq.emplace(t);
      }
    }
    return uniq;
  }

  template <typename T>
  static std::set<T> GetIntersection(const std::vector<std::set<T>>& list)
  {
    std::set<char> intersection = list[0];

    for (size_t i = 1; i < list.size(); i++)
    {
      for (auto it = intersection.begin(); it != intersection.end();)
      {
        if (list[i].find(*it) == list[i].end())
          it = intersection.erase(it);
        else
          it++;
      }
    }
    return intersection;
  }

  template <typename T>
  static std::set<T> GetIntersection(const std::set<T>& s1, const std::set<T>& s2)
  {
    std::set<T> intersection = s1;

    for (auto it = intersection.begin(); it != intersection.end();)
    {
      if (s2.find(*it) == s2.end())
        it = intersection.erase(it);
      else
        it++;
    }
    return intersection;
  }

  template <typename T>
  static bool IsSubset(const std::set<T>& smallSet, const std::set<T>& bigSet)
  {
    if (bigSet.size() < smallSet.size())
      return false;

    for (auto&& t : smallSet)
    {
      if (bigSet.find(t) == bigSet.end())
        return false;
    }
    return true;
  }

  // Input     - Puzzle input
  // State     - initial state of the path finder
  // Heuristic - int(Input, State)
  //             Given a state, what is the minimum cost to get to the goal state
  // Branch    - std::vector<std::pair<int, State>>(Input, State);
  //             Given a state, what are the possible next states and the cost to get to it
  // Goal      - bool(Input, State)
  //             Given a state, are we in the goal state
  template <typename Input, typename State, typename Heuristic, typename Branch, typename Goal>
  static int AStar(const Input& input, const std::vector<State>& initial, Heuristic heuristic, Branch branch, Goal goal)
  {
    std::map<State, int> visited;
    std::multimap<int, std::pair<int, State>> open;
    for (int i = 0; i < initial.size(); i++)
      open.emplace(0, std::pair<int, State>{0, initial[i]});
    while (!open.empty())
    {
      auto it = open.begin();
      if (goal(input, it->second.second))
      {
        return it->second.first;
      }
      int stateCost = it->second.first;
      for (auto& newState : branch(input, it->second.second))
      {
        auto it = visited.find(newState.second);
        int cost = stateCost + newState.first;
        if (it == visited.end() || it->second > cost)
        {
          open.emplace(heuristic(input, newState.second) + cost, std::pair<int, State>{cost, newState.second});
          visited.emplace(newState.second, cost);
        }
      }
      open.erase(open.begin());
    }
    return 0;
  }

  // Input     - Puzzle input
  // State     - initial state of the path finder
  // Heuristic - int(Input, State)
  //             Given a state, what is the minimum cost to get to the goal state
  // Branch    - std::vector<std::pair<int, State>>(Input, State);
  //             Given a state, what are the possible next states and the cost to get to it
  // Goal      - bool(Input, State)
  //             Given a state, are we in the goal state
  template <typename Input, typename State, typename Heuristic, typename Branch, typename Goal>
  static std::pair<int, std::vector<State>> AStarPath(const Input& input, const std::vector<State>& initial, Heuristic heuristic, Branch branch, Goal goal)
  {
    std::map<State, int> visited;
    std::multimap<int, std::pair<int, std::vector<State>>> open;
    for (int i = 0; i < initial.size(); i++)
      open.emplace(0, std::pair<int, std::vector<State>>{0, {initial[i]}});
    while (!open.empty())
    {
      const auto& [heuristicVal, state] = *open.begin();
      const auto& [cost, path] = state;

      if (goal(input, path.back()))
        return {cost, path};

      for (const auto& [stateCost, newState] : branch(input, path.back()))
      {
        auto it = visited.find(newState);
        int newCost = cost + stateCost;
        if (it == visited.end() || it->second > cost)
        {
          auto newPath = path;
          newPath.emplace_back(newState);
          open.emplace(heuristic(input, newState) + newCost, std::pair<int, std::vector<State>>{newCost, newPath});
          visited.emplace(newState, newCost);
        }
      }
      open.erase(open.begin());
    }
    return {0, {}};
  }

  // Input     - Puzzle input
  // State     - initial state of the path finder
  // Heuristic - int(Input, State)
  //             Given a state, what is the minimum cost to get to the goal state
  // Branch    - std::vector<std::pair<int, State>>(Input, State);
  //             Given a state, what are the possible next states and the cost to get to it
  // Goal      - bool(Input, State)
  //             Given a state, are we in the goal state
  template <typename Input, typename State, typename Heuristic, typename Branch, typename Goal>
  static std::pair<int, std::vector<State>> AStarPath(const Input& input, const State& initial, Heuristic heuristic, Branch branch, Goal goal)
  {
    return AStarPath(input, std::vector<State>{initial}, heuristic, branch, goal);
  }

  // Input  - Puzzle input
  // State  - State of the path finder
  // Branch - std::vector<std::pair<int, State>>(Input, State);
  //          Given a state, what are the possible next states and the cost to get to it
  // Goal   - bool(Input, State)
  //          Given a state, are we in the goal state
  template <typename Input, typename State, typename Branch, typename Goal>
  static std::pair<int, std::vector<State>> DijkstrasPath(const Input& input, const std::vector<State>& initial, Branch branch, Goal goal)
  {
    return AStarPath(input, initial, [](const Input&, const State&) { return 0; }, branch, goal);
  }

  // Input  - Puzzle input
  // State  - State of the path finder
  // Branch - std::vector<std::pair<int, State>>(Input, State);
  //          Given a state, what are the possible next states and the cost to get to it
  // Goal   - bool(Input, State)
  //          Given a state, are we in the goal state
  template <typename Input, typename State, typename Branch, typename Goal>
  static std::pair<int, std::vector<State>> DijkstrasPath(const Input& input, const State& initial, Branch branch, Goal goal)
  {
    return AStarPath(input, std::vector<State>{initial}, [](const Input&, const State&) { return 0; }, branch, goal);
  }

  // Input  - Puzzle input
  // State  - State of the path finder
  // Branch - std::vector<std::pair<int, State>>(Input, State);
  //          Given a state, what are the possible next states and the cost to get to it
  // Goal   - bool(Input, State)
  //          Given a state, are we in the goal state
  template <typename Input, typename State, typename Branch, typename Goal>
  static std::set<State> DijkstrasAllVisited(const Input& input, const State& initial, Branch branch, Goal goal)
  {
    std::set<State> totalVisited;
    int bestCost = std::numeric_limits<int>::max();
    std::map<State, int> visited;
    std::multimap<int, std::vector<State>> open;
    open.emplace(0, std::vector{initial});

    while (!open.empty())
    {
      const auto& [cost, path] = *open.begin();
      if (cost > bestCost)
      {
        open.erase(open.begin());
        continue;
      }

      if (goal(input, path.back()))
      {
        bestCost = cost;
        for (const auto& state : path)
        {
          totalVisited.emplace(state);
        }
        open.erase(open.begin());
        continue;
      }
      for (const auto& [newStateCost, newState] : branch(input, path.back()))
      {
        int newCost = cost + newStateCost;
        auto it = visited.find(newState);
        if (it == visited.end() || it->second >= newCost)
        {
          auto cpy = path;
          cpy.emplace_back(newState);
          open.emplace(newCost, cpy);
          visited.emplace(newState, newCost);
        }
      }
      open.erase(open.begin());
    }
    return totalVisited;
  }

  // Input     - Puzzle input
  // State     - initial state of the path finder
  // Heuristic - int(Input, State)
  //             Given a state, what is the minimum cost to get to the goal state
  // Branch    - std::vector<std::pair<int, State>>(Input, State);
  //             Given a state, what are the possible next states and the cost to get to it
  // Goal      - bool(Input, State)
  //             Given a state, are we in the goal state
  template <typename Input, typename State, typename Heuristic, typename Branch, typename Goal>
  static int AStar(const Input& input, const State& initial, Heuristic heuristic, Branch branch, Goal goal)
  {
    AStar(input, std::vector<State>{initial}, heuristic, branch, goal);
  }

  // Input  - Puzzle input
  // State  - State of the path finder
  // Branch - std::vector<std::pair<int, State>>(Input, State);
  //          Given a state, what are the possible next states and the cost to get to it
  // Goal   - bool(Input, State)
  //          Given a state, are we in the goal state
  template <typename Input, typename State, typename Branch, typename Goal>
  static int Dijkstras(const Input& input, const std::vector<State>& initial, Branch branch, Goal goal)
  {
    return AStar(input, initial, [](const Input&, const State&) { return 0; }, branch, goal);
  }

  // Input  - Puzzle input
  // State  - State of the path finder
  // Branch - std::vector<std::pair<int, State>>(Input, State);
  //          Given a state, what are the possible next states and the cost to get to it
  // Goal   - bool(Input, State)
  //          Given a state, are we in the goal state
  template <typename Input, typename State, typename Branch, typename Goal>
  static int Dijkstras(const Input& input, const State& initial, Branch branch, Goal goal)
  {
    return AStar(input, std::vector<State>{initial}, [](const Input&, const State&) { return 0; }, branch, goal);
  }

  template <typename Container>
  static std::ostream& Print(std::ostream& stream, const Container& container)
  {
    stream << '[';
    int i = 0;
    for (auto it = container.begin(); it != container.end(); ++it)
    {
      if (i != 0)
        stream << ", ";
      stream << *it;
      i++;
    }
    stream << ']';
    return stream;
  }

  static std::string Repeat(const std::string& str, int count)
  {
    std::string s;
    for (int i = 0; i < count; i++)
    {
      s += str;
    }
    return s;
  }

  template <typename T>
  static std::vector<T> Repeat(const std::vector<T>& vec, int count)
  {
    std::vector<T> ret;
    for (int i = 0; i < count; i++)
    {
      for (const auto& t : vec)
      {
        ret.emplace_back(t);
      }
    }
    return ret;
  }

  static std::vector<std::string> GetAllRegexMatches(const std::string& str, const std::string& regex)
  {
    std::vector<std::string> matches;
    std::regex reg{regex};
    auto it = std::sregex_iterator(str.begin(), str.end(), reg);
    auto end = std::sregex_iterator();
    for (it; it != end; it++)
    {
      matches.emplace_back(it->str());
    }
    return matches;
  }

  static std::vector<Index2D> GetNeighborDirections()
  {
    return std::vector<Index2D>{Index2D{1, 0}, Index2D{0, 1}, Index2D{-1, 0}, Index2D{0, -1}};
  }

  static Index2D GetDirection(char c)
  {
    if (c == 'v' || c == 'V') return Index2D{0, 1};
    if (c == '^')             return Index2D{0, -1};
    if (c == '>')             return Index2D{1, 0};
    if (c == '<')             return Index2D{-1, 0};
    std::cerr << "GetDirection: Invalid char: " << c << std::endl;
  }

  static int64_t ChineseRemainderTheoremTwo(int64_t mod1, int64_t mod2, int64_t res1, int64_t res2)
  {
    int64_t i = res1;
    while (true)
    {
      if (i % mod2 == res2)
        return i;

      i += mod1;
    }
    return 0;
  }

  static int64_t ChineseRemainderTheorem(const std::vector<int64_t>& mods, const std::vector<int64_t>& results)
  {
    int64_t currentMod = mods.front();
    int64_t currentRes = results.front();
    for (int i = 1; i < mods.size(); i++)
    {
      int64_t result = ChineseRemainderTheoremTwo(currentMod, mods[i], currentRes, results[i]);
      currentRes = result;
      currentMod = currentMod * mods[i];
    }
    return currentRes;
  }

private:
  template <typename Key, typename Value, typename Compare, typename Eval>
  static int TSP(const Graph<Key, Value>& graph, const std::set<Key>& nodesLeft, const std::vector<Key>& order, Compare compare, Eval eval)
  {
    if (nodesLeft.empty())
    {
      return eval(graph, order);
    }

    bool hasSet = false;
    Value tsp;

    for (auto& node : nodesLeft)
    {
      std::set<Key> newNodesLeft = nodesLeft;
      newNodesLeft.erase(node);
      std::vector<Key> newOrder = order;
      newOrder.emplace_back(node);
      if (hasSet)
        tsp = compare(tsp, TSP(graph, newNodesLeft, newOrder, compare, eval));
      else
        tsp = TSP(graph, newNodesLeft, newOrder, compare, eval);
      hasSet = true;
    }
    return tsp;
  }

public:
  // Key     - Node type of the graph
  // Value   - Edge type of the graph
  // Compare - Value(Value, Value)
  //           Given two solutions, which one is better
  // Eval    - int(const Graph&, const std::vector<Key>&)
  //           Given the graph and order of the TSP, what is the total cost
  template <typename Key, typename Value, typename Compare, typename Eval>
  static int TSP(const Graph<Key, Value>& graph, const Key& start, Compare compare, Eval eval)
  {
    std::set<Key> nodes = graph.GetNodes();
    nodes.erase(nodes.find(start));
    return TSP(graph, nodes, std::vector<Key>{start}, compare, eval);
  }

  // Key     - Node type of the graph
  // Value   - Edge type of the graph
  // Compare - Value(Value, Value)
  //           Given two solutions, which one is better
  // Eval    - int(const Graph&, const std::vector<Key>&)
  //           Given the graph and order of the TSP, what is the total cost
  template <typename Key, typename Value, typename Compare, typename Eval>
  static int TSP(const Graph<Key, Value>& graph, Compare compare, Eval eval)
  {
    return TSP(graph, graph.GetNodes(), std::vector<Key>{}, compare, eval);
  }
};

template <typename T>
static std::ostream& operator<<(std::ostream& stream, const std::vector<T>& container)
{
  return Helper::Print(stream, container);
}

template <typename T>
static std::ostream& operator<<(std::ostream& stream, const std::set<T>& container)
{
  return Helper::Print(stream, container);
}

template <typename T, typename S>
static std::ostream& operator<<(std::ostream& stream, const std::map<T, S>& container)
{
  stream << "{";
  int i = 0;
  for (const auto& [t, s] : container)
  {
    if(i != 0)
      stream << ", ";
    stream << t << " => " << s;
    i++;
  }
  stream << "}";
  return stream;
}

template <typename T, typename S>
static std::ostream& operator<<(std::ostream& stream, const std::pair<T, S>& pair)
{
  return stream << "(" << pair.first << ", " << pair.second << ")";
}
