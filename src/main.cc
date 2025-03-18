#include <iostream>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

class TimerManager
{
public:
  TimerManager() : _worker(&TimerManager::processTimers, this), _running(true) {}

  ~TimerManager()
  {
    stop();
  }

  void setTimer(int delayMs, std::function<void()> callback)
  {
    auto timePoint = std::chrono::steady_clock::now() + std::chrono::milliseconds(delayMs);

    {
      std::lock_guard<std::mutex> lock(_mutex);
      _timers.emplace(timePoint, callback);
    }
    _cv.notify_one();
  }

  void stop()
  {
    {
      std::lock_guard<std::mutex> lock(_mutex);
      _running = false;
    }
    _cv.notify_one();
    if (_worker.joinable())
      _worker.join();
  }

private:
  using Timer = std::pair<std::chrono::steady_clock::time_point, std::function<void()>>;
  struct Compare
  {
    bool operator()(const Timer &a, const Timer &b)
    {
      return a.first > b.first;
    }
  };

  std::priority_queue<Timer, std::vector<Timer>, Compare> _timers;
  std::thread _worker;
  std::mutex _mutex;
  std::condition_variable _cv;
  bool _running;

  void processTimers()
  {
    std::unique_lock<std::mutex> lock(_mutex);
    while (_running)
    {
      if (_timers.empty())
      {
        _cv.wait(lock);
      }
      else
      {
        auto now = std::chrono::steady_clock::now();
        auto nextTimer = _timers.top();

        if (nextTimer.first <= now)
        {
          _timers.pop();
          lock.unlock();
          nextTimer.second(); // Execute callback
          lock.lock();
        }
        else
        {
          _cv.wait_until(lock, nextTimer.first);
        }
      }
    }
  }
};

int main()
{
  TimerManager manager;

  std::cout << "Starting timers...\n";
  manager.setTimer(2000, []()
                   { std::cout << "Timer 1: 2 seconds elapsed!\n"; });
  manager.setTimer(1000, []()
                   { std::cout << "Timer 2: 1 second elapsed!\n"; });
  manager.setTimer(3000, []()
                   { std::cout << "Timer 3: 3 seconds elapsed!\n"; });

  std::this_thread::sleep_for(std::chrono::seconds(4));
  std::cout << "Main function finished.\n";
  return 0;
}
