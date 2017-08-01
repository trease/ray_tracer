#ifndef THREAD_SAFE_HPP
#define THREAD_SAFE_HPP
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
template<typename T>

class ThreadSafeQueue
{
public:
	void push(const T & value);
	bool empty() const;
	bool try_pop(T& popped_value);
	void wait_and_pop();
	int size();
	T front();
private:
	std::queue<T> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
	int s=0;
};


template<typename T>
bool ThreadSafeQueue<T>::empty() const {
	std::lock_guard<std::mutex> lock(the_mutex);
	return the_queue.empty();
}

template<typename T>
void ThreadSafeQueue<T>::push(const T& value) {
	std::unique_lock<std::mutex> lock(the_mutex);
	the_queue.push(value);
	s++;
	lock.unlock();
	the_condition_variable.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::try_pop(T &popped_value) {
	std::lock_guard<std::mutex> lock(the_mutex);
	popped_value = the_queue.front();
	the_queue.pop();
	return true;
}

template<typename T>
void ThreadSafeQueue<T>::wait_and_pop() {
	std::unique_lock<std::mutex> lock(the_mutex);
	while (the_queue.empty()) {
		the_condition_variable.wait(lock);
	}
	the_queue.pop();
}

template<typename T>
T ThreadSafeQueue<T>::front() {
	return the_queue.front();
}

template<typename T>
int ThreadSafeQueue<T>::size() {
	return s;
}

#endif