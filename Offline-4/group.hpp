#pragma once

#include<iostream>
#include <syncstream>
#include <pthread.h>
#include <semaphore.h>
#include<vector>

using namespace std;

class Student;

class Group{
public:
    int id;
    int size;
    sem_t print_finished_sem;
    Student* group_leader;
    vector<Student *> members;
    static vector<Group> groups;
    Group(int g_id = 0, int g_size = 0);
    void notify(int ps);

};