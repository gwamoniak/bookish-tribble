#pragma once
#include "net_common.h"

namespace net
{
	template<typename T>
	class tsqueued
	{
	public:
		tsqueued() = default;
		tsqueued(const tsqueued<T>&) = delete;
		~tsqueued() { clear(); }

		//returns and maintains item at front or back of Queue
		const T& front()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.front();
		}

		const T& back()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.back();
		}

		// push_back push_front
		void push_back(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_back(std::move(item));

			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}
		void push_front(const T& item)
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.emplace_front(std::move(item));


			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		// empty count  and clear
		bool empty()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.empty();
		}

		size_t count()
		{
			std::scoped_lock lock(muxQueue);
			return deqQueue.size();
		}
		
		void clear()
		{
			std::scoped_lock lock(muxQueue);
			deqQueue.clear();
		}

		// removes and returns item from front of Queue
		T pop_front()
		{
			std::scoped_lock lock(muxQueue);
			auto temp = std::move(deqQueue.front());
			deqQueue.pop_front();
			return temp;
		}

		T pop_back()
		{
			std::scoped_lock lock(muxQueue);
			auto temp = std::move(deqQueue.back());
			deqQueue.pop_back();
			return temp;
		}

		void wait()
		{
			while (empty())
			{
				std::unique_lock<std::mutex> ulock(muxBlocking);
				cvBlocking.wait(ulock);
			}
		}

		

	protected:
		std::mutex muxQueue;
		std::deque<T> deqQueue;
		std::condition_variable cvBlocking;
		std::mutex muxBlocking;

	};

}
