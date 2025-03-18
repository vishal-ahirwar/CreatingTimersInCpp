// Auto Genrated C++ file by aura CLI
// None
#include <iostream>
#include <timerconfig.h>
#include <thread>
#include <chrono>
#include <functional>
#include <string>
#include <vector>

class App
{
  static std::vector<std::string> _args;

public:
  static int exec(int argc, char **argv)
  {
    for (int i = 0; i < argc; ++i)
      _args.push_back(argv[i]);
    while (true)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    };
    return 0;
  };
};
std::vector<std::string> App::_args = {};
class Timer
{
  int _remaining_time{};
  std::function<void()> _callback;
  bool _bloop{};

public:
  Timer() {};
  void setCallback(std::function<void()> callback)
  {
    _callback = callback;
  }
  void start(const int &t, bool b_loop)
  {
    _bloop = b_loop;
    if (!_callback)
    {
      std::cerr << "Error : callback is not set!\n";
      return;
    }
    _remaining_time = t;
    std::thread(&Timer::run, this).detach();
  }

private:
  void run()
  {
    int current_time = _remaining_time;
    do
    {

      while (current_time--)
      {
        std::this_thread::sleep_for(std::chrono::seconds(1));
      };
      _callback();
      current_time = _remaining_time;
    } while (_bloop);
  }
};

int main(int argc, char *argv[])
{
  Timer timer1, timer2, timer3;
  timer1.setCallback([]()
                     { std::cout << "timer1\n"; });
  timer1.start(5, false);
  timer2.setCallback([]()
                     { std::cout << "timer2\n"; });
  timer2.start(1, false);
  timer3.setCallback([]()
                     { std::cout << "timer3\n"; });
  timer3.start(3, false);

  return App::exec(argc, argv);
}
