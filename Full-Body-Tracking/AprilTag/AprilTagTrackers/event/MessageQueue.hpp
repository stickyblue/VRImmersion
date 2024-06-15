#pragma once

#include "utils/Assert.hpp"

#include <mutex>
#include <vector>

namespace evt
{

/// message queue
template <typename T>
class MessageQueue
{
public:
    MessageQueue() = default;

    ~MessageQueue() noexcept
    {
        ATT_ASSERT(mWriteQueue.empty());
    }

    MessageQueue(const MessageQueue&) = delete;
    MessageQueue(MessageQueue&&) noexcept = delete;
    MessageQueue& operator=(const MessageQueue&) = delete;
    MessageQueue& operator=(MessageQueue&&) noexcept = delete;

    /// enqueue a message from any thread
    void Enqueue(T&& message) noexcept
    {
        mWriteQueue.push_back(std::move(message));
    }

    /// process the list of messages on this thread
    /// @param lock expects locked, returns unlocked
    template <typename Fn>
    void Process(std::unique_lock<std::mutex>& lock, Fn&& func)
    {
        ATT_ASSERT(lock.owns_lock());
        if (mWriteQueue.empty()) return;
        std::swap(mWriteQueue, mReadQueue);
        lock.unlock();

        for (T& message : mReadQueue)
        {
            func(message);
        }
        mReadQueue.clear();
    }

private:
    std::vector<T> mWriteQueue;
    std::vector<T> mReadQueue;
    std::mutex mMutex;
};

} // namespace evt
