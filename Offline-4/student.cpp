#include <unistd.h>
#include <iostream>
#include <syncstream>
#include "student.hpp"
#include "staff.hpp"
#include "count_timer.hpp"

pthread_mutex_t Student::printer_locks[4];
sem_t Student::binder_sem;
int Student::print_delay;
int Student::bind_delay;
int Student::submission_delay;
int Student::printer_state[NUM_PRINT_STATIONS];


void Student::initStudentClass(int w,int x, int y){
        sem_init(&Student::binder_sem,0,2);
        for(int i=0;i<NUM_PRINT_STATIONS;i++){
            pthread_mutex_init(&Student::printer_locks[i],NULL);
        }
        for(int i=0;i<NUM_PRINT_STATIONS;i++){
            printer_state[i]=FREE;
        }
        print_delay=w;
        bind_delay=x;
        submission_delay=y;
    }

Student::Student(int s_id){
        id=s_id;
        waiting=false;
        pthread_mutex_init(&has_printed_lock,NULL);
        pthread_mutex_init(&waiting_lock,NULL);
        sem_init(&wait_semaphore,0,0);
    }


int Student::leftIndex(int id){
    return (id+NUM_PRINT_STATIONS-1)%NUM_PRINT_STATIONS;
}
int Student::rightIndex(int id){
    return (id+1)%NUM_PRINT_STATIONS;
}

void Student::test(int id){
        int left=leftIndex(id);
        int right=rightIndex(id);
        if(printer_state[id]==PRE_PRINTING && printer_state[left]!=PRINTING && printer_state[right]!=PRINTING){
            printer_state[id]=PRINTING;
            //sem_post(&printer_sems[id]);
        }
}
void Student::setGroup(Group* g){
    group=g;
}
void* Student::student_phase_wrapper(void* arg) {
        Student* student = static_cast<Student*>(arg);
        return student->student_phase(arg);
}

void Student::start_thread(){

        pthread_create(&student_thread, NULL, &Student::student_phase_wrapper, this);    
}

void* Student::student_phase(void* arg){
        sleep(Clock::student_random_delay());
        int assigned_print_station=id%4;
        std::osyncstream(std::cout) << "Student " << id << " has arrived at printing station at time "  <<Clock::get_seconds()<< std::endl;

        pthread_mutex_lock(&printer_locks[assigned_print_station]);
        if(printer_state[assigned_print_station]==FREE){
            printer_state[assigned_print_station]=BUSY;
            pthread_mutex_unlock(&printer_locks[assigned_print_station]);
        }
        else{
            pthread_mutex_unlock(&printer_locks[assigned_print_station]);
            pthread_mutex_lock(&waiting_lock);
            waiting=true;
            pthread_mutex_unlock(&waiting_lock);
            sem_wait(&wait_semaphore);
        }

        //eat
        sleep(print_delay);
        
        std::osyncstream(std::cout) << "Student " << id << " has finished printing at time "  <<Clock::get_seconds()<< std::endl;
       
        pthread_mutex_lock(&printer_locks[assigned_print_station]);
        printer_state[assigned_print_station]=FREE;
        pthread_mutex_unlock(&printer_locks[assigned_print_station]);
        
        group->notify(assigned_print_station);
        sem_post(&(group->print_finished_sem));
        if(group->group_leader->id != id) return NULL;
        // std::osyncstream(std::cout)<<"gleader"<<group->group_leader->id<<std::endl;;
        // std::osyncstream(std::cout)<<"bop"<<std::endl;
        for(int i=0;i<group->members.size();i++){
            sem_wait(&(group->print_finished_sem));
        }
        std::osyncstream(std::cout) << "Group " << group->id << " has finished printing at time "  <<Clock::get_seconds()<< std::endl;

        sem_wait(&binder_sem);

        std::osyncstream(std::cout) << "Group " << group->id << " has started binding at time "  <<Clock::get_seconds()<< std::endl;
        sleep(bind_delay);

        sem_post(&binder_sem);

        std::osyncstream(std::cout) << "Group " << group->id << " has finished binding at time "  <<Clock::get_seconds()<< std::endl;

        pthread_mutex_lock(&Staff::write_entry_lock);
        Staff::num_submission++;
        sleep(submission_delay);
        pthread_mutex_unlock(&Staff::write_entry_lock);

        std::osyncstream(std::cout) << "Group " << group->id << " has submitted report at time "  <<Clock::get_seconds()<< std::endl;

        return NULL;

}
bool Student::release_print_if_waiting(){
        bool to_return = false;
        pthread_mutex_lock(&waiting_lock);

        if(waiting)
        {
        to_return = true;
        waiting = false;

        sem_post(&wait_semaphore);
        }

        pthread_mutex_unlock(&waiting_lock);
        return to_return;
}