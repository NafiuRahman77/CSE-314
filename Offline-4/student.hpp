#pragma once

#include <iostream>
#include <syncstream>
#include <random>
#include <pthread.h>
#include <semaphore.h>
#include "group.hpp"
#include "defs.hpp"


class Student{
public:
    int id;
    pthread_t student_thread;
    Group* group;  
    bool has_printed;
    bool waiting;
    pthread_mutex_t has_printed_lock;
    pthread_mutex_t waiting_lock;
    sem_t wait_semaphore;
    static int print_delay, bind_delay, submission_delay;
    static sem_t binder_sem;
    static pthread_mutex_t printer_locks[NUM_PRINT_STATIONS];
    static int printer_state[NUM_PRINT_STATIONS];
    
    static void initStudentClass(int w,int x, int y);
    Student(int s_id);
    int leftIndex(int id);
    int rightIndex(int id);
    void test(int id);
    void setGroup(Group* g);
    static void* student_phase_wrapper(void* arg) ;
    void start_thread();
    void* student_phase(void* arg);
    bool release_print_if_waiting();
        
};


