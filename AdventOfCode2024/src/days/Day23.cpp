#include "aoc.h"

namespace day23
{
  struct Network
  {
    std::map<std::string, std::set<std::string>> graph;
  };

  REGISTER_DAY(day23, Network, int64_t);

  REGISTER_TEST_EXAMPLE(day23, ExampleInput, 1, 7);
  REGISTER_TEST        (day23, Input,        1, 1170);
  REGISTER_TEST_EXAMPLE(day23, ExampleInput, 2, 52);
  REGISTER_TEST        (day23, Input,        2, 350);

  READ_INPUT(input)
  {
    Network network;
    std::string str;
    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      std::string s1, s2;
      std::getline(ss, s1, '-');
      std::getline(ss, s2, '-');
      network.graph[s1].emplace(s2);
      network.graph[s2].emplace(s1);
    }
    return network;
  }

  void FindLargest(const Network& network, std::set<std::string>& computers, std::set<std::string>& largest)
  {
    if (computers.size() <= largest.size())
      return;

    for (const auto& computer : computers)
    {
      for (const auto& otherComputer : computers)
      {
        if (otherComputer == computer)
          continue;

        const auto& neighbors = network.graph.find(computer)->second;
        if (neighbors.find(otherComputer) == neighbors.end())
        {
          // Solution wasn't found, so remove an element and try for a smaller set of computers
          for (const auto& node : computers)
          {
            auto cpy = computers;
            cpy.erase(node);
            FindLargest(network, cpy, largest);
          }
          return;
        }
      }
    }
    largest = computers;
  }

  OUTPUT1(input)
  {
    std::set<std::set<std::string>> groupsOfThree;
    for (const auto& computer : input.graph)
    {
      if (computer.first[0] != 't')
        continue;

      for (auto it = computer.second.begin(); it != computer.second.end(); it++)
      {
        auto it2 = it;
        it2++;
        for (it2; it2 != computer.second.end(); it2++)
        {
          const auto& paths = input.graph.find(*it)->second;
          if (paths.find(*it2) != paths.end())
            groupsOfThree.emplace(std::set<std::string>{computer.first, *it, *it2});
        }
      }
    }
    return groupsOfThree.size();
  }

  OUTPUT2(input)
  {
    std::set<std::string> largest;
    for (const auto& computer : input.graph)
    {
      std::set<std::string> computers = computer.second;
      computers.emplace(computer.first);
      FindLargest(input, computers, largest);
    }

    for (const auto& s : largest)
      std::cout << s << ",";
    std::cout << std::endl;

    // Some arbitrary solution number to have some form of verification in the code.
    // Sum all letters of the computers (with a=0, b=1, etc)
    int64_t sum = 0;
    for (const auto& s : largest)
      sum += (s[0] - 'a') + (s[1] - 'a');

    return sum;
  }
}
