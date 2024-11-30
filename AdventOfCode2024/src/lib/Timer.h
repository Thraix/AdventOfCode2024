#pragma once

#include <chrono>

class Timer
{
private:
  using Time = std::chrono::steady_clock;
  using TimeStamp = std::chrono::time_point<std::chrono::steady_clock>;
  using Duration = std::chrono::duration<double>;

  TimeStamp past;
  TimeStamp current;

public:
  Timer()
    : past{Time::now()}
  {
  }

  void Start()
  {
    past = Time::now();
  }

  void Stop()
  {
    current = Time::now();
  }

  double Elapsed()
  {
    return Duration{current - past}.count();
  }
};
