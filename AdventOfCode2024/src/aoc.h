#pragma once

#include "lib/Array2D.h"
#include "lib/Array3D.h"
#include "lib/Array4D.h"
#include "lib/Graph.h"
#include "lib/Helper.h"
#include "lib/Input.h"
#include "lib/Json.h"
#include "lib/Mapper.h"
#include "lib/Math.h"
#include "lib/Timer.h"

#include <algorithm>
#include <bitset>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#define TERM_RED "\x1B[31m"
#define TERM_GREEN "\x1B[32m"
#define TERM_CLEAR "\033[0m"

namespace aoc
{
  template <typename InputType, typename OutputType>
  class Test
  {
  private:
    std::string name;
    std::string inputFile;
    OutputType expectedOutput;
    bool isExample = false;
    int part = 0;

  public:
    Test(const std::string& name, const std::string& inputFile, const OutputType& expectedOutput, bool isExample, int part)
    :  name {name},
       inputFile{inputFile},
       expectedOutput{expectedOutput},
       isExample{isExample},
       part{part}
    {}

    const std::string& GetInputFile() const { return inputFile;  }
    const std::string& GetName() const { return name;  }
    const OutputType& GetExpectedOutput() const { return expectedOutput;  }
    bool IsExample() const { return isExample; }
    int GetPart() const { return part; }
  };

  class Day
  {
  public:
    virtual bool AocRunTestCases() = 0;
    virtual const std::string& GetName() const = 0;
  };

  template<typename InputType, typename OutputType>
  class DayT : public Day {
  protected:
    std::vector<Test<InputType, OutputType>*> tests;

    std::string name;
    bool isExample = true;
  public:
    DayT(const std::string& name)
      : name{name} 
    {
      this->name[0] = std::toupper(name[0]);
    }

    void AddTest(Test<InputType, OutputType>* test)
    {
      tests.emplace_back(test);
    }

    bool AocRunTestCases() override
    {
      for (const auto& test : tests)
      {
        isExample = test->IsExample();
        std::ifstream inputStream{test->GetInputFile()};
        if (!inputStream.is_open())
        {
          std::cout << TERM_RED << "Could not find input file: " << test->GetInputFile() << TERM_CLEAR << std::endl;
          return false;
        }
        std::string testName = name + ".P" + std::to_string(test->GetPart()) + "." + test->GetName();
        std::cout << TERM_GREEN << "[ RUN     ] " << TERM_CLEAR << testName << std::endl;
        InputType input = ReadInput(inputStream);
        OutputType output = test->GetPart() == 1 ? Output1(input) : Output2(input);

        if (!Check(test->GetExpectedOutput(), output, testName))
        {
          return false;
        };
      }
      return true;
    }

    const std::string& GetName() const override
    {
      return name;
    }

    virtual InputType ReadInput(std::istream& input) = 0;
    virtual OutputType Output1(const InputType& input) = 0;
    virtual OutputType Output2(const InputType& input) = 0;

  private:
    bool Check(const OutputType& expected, const OutputType& actual, const std::string& name) const
    {
      if (expected == actual)
      {
        std::cout << TERM_GREEN << "[      OK ] " << TERM_CLEAR << "  Result: " << actual << std::endl;
        return true;
      }
      else
      {
        std::cout << TERM_RED << "[         ] " << TERM_CLEAR << "Unexpected value in " << name << std::endl;
        std::cout << TERM_RED << "[         ] " << TERM_CLEAR << "  expect: " << expected << std::endl;
        std::cout << TERM_RED << "[  FAILED ] " << TERM_CLEAR << "  actual: " << actual << std::endl;
        return false;
      }
    }
  };

  struct ptr_compare
  {
    using is_transparent = void;
    bool operator()(const Day* lhs, const Day* rhs) const
    {
      return lhs->GetName() < rhs->GetName();
    };

    bool operator()(const Day* lhs, const std::string& rhs) const
    {
      return lhs->GetName() < rhs;
    };

    bool operator()(const std::string& lhs, const Day* rhs) const
    {
      return lhs < rhs->GetName();
    };
  };

  class Registry
  {
  private:
    using Days = std::set<Day*, ptr_compare>;
    Days days;

    Day* AddDay(Day* day)
    {
      days.emplace(day);
      return day;
    }
    
    template <typename InputType, typename OutputType>
    Test<InputType, OutputType>* AddTest(std::string day, Test<InputType, OutputType>* test)
    {
      day[0] = std::toupper(day[0]);
      auto it = days.find(day);
      if(it == days.end())
      {
        std::cout << TERM_RED << "Could not find Day: " << day << TERM_CLEAR << std::endl;
        return test;
      }
      DayT<InputType, OutputType>* dayT = dynamic_cast<DayT<InputType, OutputType>*>(*it);
      if (!dayT)
      {
        std::cout << TERM_RED << "Failed to cast day: " << day << TERM_CLEAR << std::endl;
        return test;
      }
      dayT->AddTest(test);
      return test;
    }


  public:
    template<typename DayImpl>
    static DayImpl* RegisterDay(DayImpl* day)
    {
      return (DayImpl*)GetInstance()->AddDay(day);
    }

    template <typename InputType, typename OutputType>
    static Test<InputType, OutputType>* RegisterTest(std::string day, Test<InputType, OutputType>* test)
    {
      return GetInstance()->AddTest(day, test);
    }

    static Registry* GetInstance();

    static Days& GetDays() { return GetInstance()->days; }
  };
};

#define DAY_STR(day) #day

#define REGISTER_DAY(day, InputTypeType, OutputTypeType)                                   \
using InputType = InputTypeType;                                                           \
using OutputType = OutputTypeType;                                                         \
class DayImpl : public aoc::DayT<InputType, OutputType>                                    \
{                                                                                          \
public:                                                                                    \
  DayImpl()                                                                                \
    : DayT(DAY_STR(day))                                                                   \
  {}                                                                                       \
  InputType ReadInput(std::istream& input) override;                                       \
  OutputType Output1(const InputType& input) override;                                     \
  OutputType Output2(const InputType& input) override;                                     \
  static DayImpl* test;                                                                    \
};                                                                                         \
DayImpl* DayImpl::test = aoc::Registry::RegisterDay(new DayImpl())

#define READ_INPUT(x) InputType DayImpl::ReadInput(std::istream& x)
#define OUTPUT1(x) OutputType DayImpl::Output1(const InputType& x)
#define OUTPUT2(x) OutputType DayImpl::Output2(const InputType& x)

#define TEST_NAME_EXAMPLE(day, name, part) day##name##example##part
#define TEST_NAME(day, name, part) day##name##example##part
#define _INTERNAL_REGISTER_TEST_FULL(day, name, part, val, testName, inputFile, isExample) \
static aoc::Test<InputType, OutputType>* testName = aoc::Registry::RegisterTest(#day, new aoc::Test<InputType, OutputType>(#name, inputFile, val, isExample, part))

#define REGISTER_TEST_EXAMPLE(day, name, part, val)\
  _INTERNAL_REGISTER_TEST_FULL(day, name, part, val, TEST_NAME_EXAMPLE(day, name, part), "res/"#day"/test_input.txt", true)
#define REGISTER_TEST_EXAMPLE_FILE(day, name, part, val, inputFileName)\
  _INTERNAL_REGISTER_TEST_FULL(day, name, part, val, TEST_NAME_EXAMPLE(day, name, part), "res/"#day"/" + std::string(inputFileName), true)

#define REGISTER_TEST(day, name, part, val) \
  _INTERNAL_REGISTER_TEST_FULL(day, name, part, val, TEST_NAME(day, name, part), "res/"#day"/input.txt", false)
#define REGISTER_TEST_FILE(day, name, part, val, inputFileName) \
  _INTERNAL_REGISTER_TEST_FULL(day, name, part, val, TEST_NAME(day, name, part), "res/"#day"/" + std::string(inputFileName), false)
