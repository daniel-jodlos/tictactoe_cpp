#ifndef Blocking_Queue_h
#define Blocking_Queue_h

#include <condition_variable>
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
    std::lock_guard<std::mutex> lock(guard);
    _data.push_back(t);
  }

  variable.notify_one();
}

template <typename T> T BlockingQueue<T>::pop() noexcept {
  std::unique_lock<std::mutex> lock(guard);
  while (_data.empty()) {
    variable.wait(lock);
  }
  T result = _data.front();
  _data.pop_front();
  return result;
}

#endif