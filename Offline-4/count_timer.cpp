#include "count_timer.hpp"

std::chrono::high_resolution_clock::time_point Clock::start_time = std::chrono::high_resolution_clock::now();
std::mt19937_64 Clock::mt(77);
std::poisson_distribution<uint64_t> Clock::p_d(2.5);
pthread_mutex_t Clock::rand_lock;

void Clock::initClockClass(){
        
    pthread_mutex_init(&Clock::rand_lock,NULL);
        
}

uint64_t Clock::get_seconds()
{
    std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);

    return duration.count();
}
uint64_t Clock::student_random_delay()
{
    pthread_mutex_lock(&rand_lock);

    uint64_t to_return = p_d(mt);

    pthread_mutex_unlock(&rand_lock);

    return to_return%4+1;
}
uint64_t Clock::staff_random_delay(int w, int x,int y)
{
    pthread_mutex_lock(&rand_lock);

    uint64_t to_return = p_d(mt);

    pthread_mutex_unlock(&rand_lock);

    return to_return%(w+x+y)+(w+x+y);
}

