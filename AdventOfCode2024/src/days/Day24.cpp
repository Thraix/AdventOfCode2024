#include "aoc.h"

namespace day24
{
  struct Gate
  {
    std::string left;
    std::string right;
    std::string op;
    std::string res;
  };

  struct Gates
  {
    std::map<std::string, int> gates;
    std::vector<Gate> gateOperation;
  };

  REGISTER_DAY(day24, Gates, int64_t);

  REGISTER_TEST_EXAMPLE(day24, ExampleInput, 1, 2024);
  REGISTER_TEST        (day24, Input,        1, 57344080719736);
  // REGISTER_TEST_EXAMPLE(day24, ExampleInput, 2, 0);
  REGISTER_TEST        (day24, Input,        2, 2328);

  int GetIndex(std::map<std::string, int>& gateIndices, std::vector<int>& gates, const std::string& wire)
  {
    auto it = gateIndices.find(wire);
    if (it == gateIndices.end())
    {
      gateIndices.emplace(wire, gates.size());
      gates.emplace_back(-1);
    }
    else
    {
      return it->second;
    }
  }

  READ_INPUT(input)
  {
    std::string str;
    Gates gates;
    while (std::getline(input, str))
    {
      if (str.empty())
        break;
      std::stringstream ss{str};
      std::string name;
      int val;
      ss >> name >> val;
      name.pop_back(); // Remove ':'
      gates.gates.emplace(name, val);
    }

    while (std::getline(input, str))
    {
      std::stringstream ss{str};
      std::string left;
      std::string right;
      std::string op;
      std::string res;
      ss >> left >> op >> right >> "->" >> res;

      Gate gate{left, right, op, res};
      gates.gates.emplace(res, -1);
      gates.gateOperation.emplace_back(gate);
    }
    return gates;
  }

  int GetVal(Gates& gates, const std::string& wire)
  {
    return gates.gates[wire];
  }

  void SetVal(Gates& gates, const std::string& wire, int val)
  {
    gates.gates[wire] = val;
  }

  void SetInput(Gates& gates, int64_t x, int64_t y)
  {
    for (auto& wire : gates.gates)
    {
      if (wire.first[0] == 'x')
      {
        wire.second = x & 1;
        x >>= 1;
      }
      if (wire.first[0] == 'y')
      {
        wire.second = y & 1;
        y >>= 1;
      }
    }
  }

  std::string IndexToStr(int i)
  {
    std::string s;
    if (i < 10)
      s += "0";
    s += std::to_string(i);
    return s;
  }

  void SwapOutputs(Gates& gates, const std::string& output1, const std::string& output2, std::set<std::string>& swapped)
  {
    int i1 = -1;
    int i2 = -1;
    for (int i = 0; i < gates.gateOperation.size(); i++)
    {
      if (gates.gateOperation[i].res == output1) i1 = i;
      if (gates.gateOperation[i].res == output2) i2 = i;
    }

    std::swap(gates.gateOperation[i1].res, gates.gateOperation[i2].res);
    swapped.emplace(gates.gateOperation[i1].res);
    swapped.emplace(gates.gateOperation[i2].res);
  }

  std::string Find(const Gates& gates, const std::string& left, const std::string& right, const std::string& op)
  {
    for (const auto& gate : gates.gateOperation)
    {
      if (gate.op == op)
      {
        if ((gate.left == left && gate.right == right) || (gate.left == right && gate.right == left))
        {
          return gate.res;
        }
      }
    }
    return "";
  }

  OUTPUT1(input)
  {
    // Too low: 1972354943
    Gates cpy = input;
    bool somethingChanged = true;
    while (somethingChanged)
    {
      somethingChanged = false;
      for (const auto& gate : cpy.gateOperation)
      {
        int leftVal = GetVal(cpy, gate.left);
        int rightVal = GetVal(cpy, gate.right);
        if (leftVal != -1 && rightVal != -1 && GetVal(cpy, gate.res) == -1)
        {
          somethingChanged = true;
          if (gate.op == "AND")
          {
            SetVal(cpy, gate.res, leftVal & rightVal);
          }
          else if (gate.op == "OR")
          {
            SetVal(cpy, gate.res, leftVal | rightVal);
          }
          else if (gate.op == "XOR")
          {
            SetVal(cpy, gate.res, leftVal ^ rightVal);
          }
        }
      }
    }

    int64_t num = 0;
    for (const auto& wire : cpy.gates)
    {
      if (wire.first[0] == 'z')
      {
        int pos = std::stoi(wire.first.substr(1));
        num |= ((int64_t)wire.second << pos);
      }
    }

    return num;
  }

  OUTPUT2(input)
  {
    Gates gates = input;
    std::set<std::string> outputs;
    for (const auto& gate : gates.gateOperation)
    {
      outputs.emplace(gate.res);
    }

    std::string carry{Find(gates, "x00", "y00", "AND")};

    std::set<std::string> swapped;
    for (int i = 1; i < 46; i++)
    {
      std::string index = IndexToStr(i);
      std::string x = "x" + index;
      std::string y = "y" + index;
      std::string z = "z" + index;

      std::string xorIn = Find(gates, x, y, "XOR");
      std::string res = Find(gates, carry, xorIn, "XOR");
      if (res.empty())
      {
        for (const auto& output : outputs)
        {
          std::string res = Find(gates, carry, output, "XOR");
          if (res == z)
          {
            SwapOutputs(gates, xorIn, output, swapped);
            xorIn = output;
            break;
          }
        }
      }
      else if (res != z)
      {
        SwapOutputs(gates, res, z, swapped);
      }

      std::string andIn = Find(gates, x, y, "AND");
      std::string andXorInCarry = Find(gates, xorIn, carry, "AND");
      carry = Find(gates, andIn, andXorInCarry, "OR");
    }

    for (const auto& swap : swapped)
    {
      std::cout << swap << ",";
    }
    std::cout << std::endl;

    // Some arbitrary solution number to have some form of verification in the code.
    // Sum all letters in the swapped outputs
    int64_t sum = 0;
    for (const auto& s : swapped)
    {
      for (char c : s)
      {
        sum += c = 'a';
      }
    }

    return sum;
  }
}
