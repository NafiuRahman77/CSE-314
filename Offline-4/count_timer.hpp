#pragma once
#include <iostream>
#include <random>
#include <chrono>


class Clock{
public:
    static std::chrono::high_resolution_clock::time_point start_time;
    static pthread_mutex_t rand_lock;
    static std::mt19937_64 mt;
    static std::poisson_distribution<uint64_t> p_d;
    static void initClockClass();
    static uint64_t get_seconds();
    static uint64_t student_random_delay();
    static uint64_t staff_random_delay(int w, int x,int y);
};