#include <iostream>
#include <syncstream>
#include "staff.hpp"
#include "count_timer.hpp"

pthread_mutex_t Staff::reader_count_lock; //mutex in slide
pthread_mutex_t Staff::write_entry_lock; //db in slide
int Staff::num_submission;
int Staff::reader_count;
int Staff::print_delay;
int Staff::bind_delay;
int Staff::submission_delay;

Staff::Staff(int st_id){
        id=st_id;
   }

void  Staff::initStaffClass(int w, int x, int y){
        pthread_mutex_init(&Staff::reader_count_lock,NULL);
        pthread_mutex_init(&Staff::write_entry_lock,NULL);
        num_submission=0;
        reader_count=0 ;        
        print_delay=w;
        bind_delay=x;
        submission_delay=y;

   }

void* Staff::staff_phase_wrapper(void* arg) {
        Staff* staff = static_cast<Staff*>(arg);
        return staff->staff_phase(arg);
    }

void Staff::start_thread(){

        pthread_create(&staff_thread, NULL, &Staff::staff_phase_wrapper, this);    
    }

void* Staff::staff_phase(void* arg){
    while(true){
        pthread_mutex_lock(&reader_count_lock);
        reader_count++;
        if(reader_count==1) pthread_mutex_lock(&write_entry_lock);
        pthread_mutex_unlock(&reader_count_lock);

        sleep(submission_delay);
        
        std::osyncstream(std::cout) << "Staff " << id<< " has started reading the entry book at time "  <<Clock::get_seconds()<< ". No. of submission = " << num_submission << std::endl;

        pthread_mutex_lock(&reader_count_lock);
        reader_count--;
        if(reader_count==0) pthread_mutex_unlock(&write_entry_lock);
        pthread_mutex_unlock(&reader_count_lock);

        sleep(Clock::staff_random_delay(print_delay,bind_delay,submission_delay));
    }

   }

// void Staff::submit(){
//       pthread_mutex_lock(&write_entry_lock);
//       num_submission++;
//       sleep(submission_delay);
//       pthread_mutex_unlock(&write_entry_lock);
// }