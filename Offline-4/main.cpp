#include<iostream>
#include<fstream>
#include <syncstream>
#include<vector>
#include "student.hpp"
#include "group.hpp"
#include "staff.hpp"
#include "count_timer.hpp"

using namespace std;

int main(){
    ifstream in("input.txt");
    
    if (!in) {
        cout<<"This file doesn't exist"<<endl;
    }

    int N,M,w,x,y;
    in>>N>>M>>w>>x>>y;
    in.close();

    std::ofstream output_file("output.txt"); 
    if (!output_file.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    
    std::streambuf* original_cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(output_file.rdbuf());

    //initialize locks
    Clock::initClockClass();
    Student::initStudentClass(w,x,y);
    Staff::initStaffClass(w,x,y);

    
    in.close();


    vector<Student> students;
    // vector<Group> &groups=Group::groups;
    vector<Staff> staffs;
    Group::groups.resize(N/M);

    for(int i = 0; i < N; ++i)
    {
        Student s(i+1);
        students.push_back(s);
    }

    int k=0;
    for(int i = 0; i < N; ++i)
    {
        if((i+1)%M==0){

            Group::groups[k].id=k+1;
            Group::groups[k].group_leader=&students[i];
            for(int j=i+1-M;j<i+1;j++){
                students[j].setGroup(&Group::groups[k]);
                Group::groups[k].members.push_back(&students[j]);
            }

            //Group::groups[k].size=Group::groups[k].members.size();
            k++;       
            
        }
    }
   
    for(int i = 0; i < 2; i++)

    {
        Staff s(i+1);
        staffs.push_back(s);
    }

    for(int i = 0; i < N; i++)
    {
        students[i].start_thread();
    }
    for(int i = 0; i < 2; i++)
    {
        staffs[i].start_thread();
    }
    for(int i = 0; i < N; i++)
    {
        pthread_join(students[i].student_thread,NULL);
    }
    for(int i = 0; i < 2; i++)
    {
        pthread_join(staffs[i].staff_thread,NULL);
    }
    
    return 0;
}

