/**
 * Copyright 2015 University Corporation for Atmospheric Research. All rights
 * reserved. See the the file COPYRIGHT in the top-level source-directory for
 * licensing conditions.
 *
 *   @file: RetxThreads.cpp
 * @author: Steven R. Emmerson
 *
 * This file implements a thread-safe container for retransmission threads.
 */

#include "RetxThreads.h"

#include <mutex>

void RetxThreads::add(pthread_t& thread)
{
    std::unique_lock<std::mutex> lock(mutex);
    threads.push_front(thread);
}

void RetxThreads::remove(pthread_t& thread)
{
    class Equals {
    public:
        Equals(pthread_t& thread) : thread(thread) {}
        bool operator() (const pthread_t& other) const {
            return pthread_equal(thread, other);
        }
    private:
        const pthread_t thread;
    };
    std::unique_lock<std::mutex> lock(mutex);
    threads.remove_if(Equals(thread));
}

void RetxThreads::shutdown()
{
    std::unique_lock<std::mutex> lock(mutex);
    for (auto it = threads.begin(); it != threads.end(); ++it)
        (void)pthread_cancel(*it);
    (void)threads.empty();
}
