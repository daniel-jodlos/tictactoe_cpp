#ifndef Blocking_Queue_h
#define Blocking_Queue_h

#include <condition_variable>
#include <iostream>
#include <list>
#include <mutex>

template <typename T> class BlockingQueue {
  std::list<T> _data;
  mutable std::mutex guard;
  std::condition_variable variable;

public:
  void push(T &) noexcept;
  T pop() noexcept;
};

template <typename T> void BlockingQueue<T>::push(T &t) noexcept {
  {
    std::unique_lock<std::mutex> lock(guard);
    _data.push_back(t);
    std::clog << "Pushed to queue\n";
  }

  variable.notify_one();
}

template <typename T> T BlockingQueue<T>::pop() noexcept {
  std::clog << "Pop called, trying to acquire lock\n";
  std::unique_lock<std::mutex> lock(guard);
  std::clog << "Lock acquired\n";
  while (_data.empty()) {
    variable.wait(lock);
  }
  T result = _data.front();
  _data.pop_front();
  std::clog << "Poped front\n";
  return result;
}

#endif