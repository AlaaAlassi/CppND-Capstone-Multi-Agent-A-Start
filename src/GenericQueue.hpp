#include <mutex>
#include <deque>
#include <condition_variable>
#include"Robot.hpp"


template <class T>
class GenericQueue
{
public:
    void send(T && msg);
    T receive();

private:
std::mutex _mux;
std::condition_variable _cond;
std::deque<std::shared_ptr<T>> _queue;
};


template <typename T>
T GenericQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mux);
    _cond.wait(uLock, [this]
               { return !_queue.empty(); });
    T msg = std::move(_queue.front());
    _queue.pop_front();
    return msg;
}

template <typename T>
void GenericQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lGuard(_mux);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}