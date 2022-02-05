#include <mutex>
#include <deque>
#include <condition_variable>
#include"Robot.hpp"


template <class T>
class MessageQueue
{
public:
    void send(T && msg);
    T receive();

private:
std::mutex _mux;
std::condition_variable _cond;
std::deque<std::shared_ptr<Robot>> _message;
};


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mux);
    _cond.wait(uLock, [this]
               { return !_message.empty(); });
    T msg = std::move(_message.back());
    _message.pop_back();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lGuard(_mux);
    _message.push_back(std::move(msg));
    _cond.notify_one();
}