#pragma once

#include <iostream>
#include <syncstream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


class Staff{
public:
   int id;
   pthread_t staff_thread;
   static int print_delay, bind_delay, submission_delay;
   static int num_submission;
   static int reader_count; //rc in slide
   static pthread_mutex_t reader_count_lock; //mutex in slide
   static pthread_mutex_t write_entry_lock; //db in slide

   Staff(int st_id);

   static void initStaffClass(int w, int x, int y);

   static void* staff_phase_wrapper(void* arg) ;

   void start_thread();

   void* staff_phase(void* arg);

   static void submit();

};