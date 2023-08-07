#include "group.hpp"
#include "student.hpp"

std::vector<Group> Group::groups;
 
Group::Group(int g_id, int g_size){
        id=g_id;
        size=g_size;
        sem_init(&print_finished_sem,0,0);
    }   

void Group::notify(int ps)
{   

    for(int i = 0; i < members.size(); i++)
    {
        int member_target_printer_id = members[i]->id % 4;

        if(member_target_printer_id != ps )
        {
            continue;
        }
        else{
            if(members[i]->release_print_if_waiting())
            {
                return;
            }
        }
        
    }

    for(int i = 0; i < groups.size(); i++)
    {
        if(&groups[i] == this) continue;
        else
        {
            for(int j = 0; j < groups[i].members.size(); ++j)
            {
                int member_target_printer_id = groups[i].members[j]->id % 4;

                if(member_target_printer_id != ps )
                {
                    continue;
                }

                else{
                    
                    if( groups[i].members[j]->release_print_if_waiting())
                    {
                        return;
                    }
                }
                
            }
        }
    }
}
