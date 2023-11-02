#include "oslabs.h"
#include<stdio.h>
struct PCB NULLPCB = {0,0,0,0,0,0,0};


struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    int sum = current_process.total_bursttime+current_process.execution_endtime+current_process.execution_starttime+current_process.remaining_bursttime+current_process.process_priority+current_process.process_id+current_process.arrival_timestamp;
    if(sum==0){
    //if(current_process.total_bursttime==0&&current_process.execution_endtime==0&&current_process.remaining_bursttime==0&&current_process.arrival_timestamp==0&&current_process.process_id==0&&current_process.process_priority==0&&current_process.execution_starttime==0){
        //current_process.total_bursttime+current_process.execution_endtime+current_process.execution_starttime+current_process.remaining_bursttime+current_process.process_priority+current_process.process_id+current_process.arrival_timestamp==0
        new_process.execution_starttime=timestamp;
        new_process.execution_endtime=timestamp+new_process.total_bursttime;
        new_process.remaining_bursttime=new_process.total_bursttime;
        current_process=new_process;
        return new_process;
    }
    else if(current_process.process_priority<=new_process.process_priority){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime=new_process.total_bursttime;
        ready_queue[*queue_cnt]=new_process;
        ++(*queue_cnt);
        return current_process;

    }
    else{
        new_process.execution_starttime=timestamp;
        new_process.execution_endtime=timestamp+new_process.total_bursttime;
        new_process.remaining_bursttime=new_process.total_bursttime;
        current_process.remaining_bursttime = current_process.execution_endtime-timestamp;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt]=current_process;
        ++(*queue_cnt);
        return new_process;

    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    if(*queue_cnt == 0){return NULLPCB;}
    else{
        int highest_priority_index=0;
        int highest_priority=ready_queue[0].process_priority;
        for(int i =0;i<*queue_cnt;i++){
            if(ready_queue[i].process_priority<highest_priority){
                highest_priority_index=i;
                highest_priority = ready_queue[i].process_priority;
            }
        }
        struct PCB next = ready_queue[highest_priority_index];
        next.execution_starttime = timestamp;
        next.execution_endtime = timestamp+next.remaining_bursttime;
        for(int i = highest_priority_index;i<(*queue_cnt)-1;i++){
            ready_queue[i] = ready_queue[i+1];
        }
        --(*queue_cnt);
        return next;

    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp){
    int sum = current_process.total_bursttime+current_process.execution_endtime+current_process.execution_starttime+current_process.remaining_bursttime+current_process.process_priority+current_process.process_id+current_process.arrival_timestamp;
    if(sum==0){
        new_process.execution_starttime=time_stamp;
        new_process.execution_endtime=time_stamp+new_process.total_bursttime;
        new_process.remaining_bursttime=new_process.total_bursttime;
        current_process=new_process;
        return new_process;
    }
    else if(new_process.remaining_bursttime>=current_process.remaining_bursttime){
        new_process.execution_starttime=0;
        new_process.execution_endtime=0;
        new_process.remaining_bursttime=new_process.total_bursttime;
        ready_queue[*queue_cnt]=new_process;
        ++(*queue_cnt);
        return current_process;
    }
    else{
        new_process.execution_starttime=time_stamp;
        new_process.execution_endtime=time_stamp+new_process.total_bursttime;
        new_process.remaining_bursttime=new_process.total_bursttime;
        current_process.remaining_bursttime = current_process.execution_endtime-time_stamp;
        current_process.execution_endtime=0;
        current_process.execution_starttime=0;
        ready_queue[*queue_cnt]=current_process;
        ++(*queue_cnt);
        return new_process;

    }
}
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    if(*queue_cnt == 0){return NULLPCB;}
    else{
        int smallest_remaining_index=0;
        int smallest_remaining=ready_queue[0].remaining_bursttime;
        for(int i =0;i<*queue_cnt;i++){
            if(ready_queue[i].remaining_bursttime<smallest_remaining){
                smallest_remaining_index=i;
                smallest_remaining = ready_queue[i].remaining_bursttime;
            }
        }
        struct PCB next = ready_queue[smallest_remaining_index];
        next.execution_starttime = timestamp;
        next.execution_endtime = timestamp+next.remaining_bursttime;
        for(int i = smallest_remaining_index;i<(*queue_cnt)-1;i++){
            ready_queue[i] = ready_queue[i+1];
        }
        --(*queue_cnt);
        return next;
    }
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    int sum = current_process.total_bursttime+current_process.execution_endtime+current_process.execution_starttime+current_process.remaining_bursttime+current_process.process_priority+current_process.process_id+current_process.arrival_timestamp;
    if(sum==0){
        new_process.execution_starttime=timestamp;
        new_process.execution_endtime=timestamp+new_process.total_bursttime;
        new_process.remaining_bursttime=new_process.total_bursttime;
        current_process=new_process;
        return new_process;
    }
    else{
        new_process.execution_starttime=0;
        new_process.execution_endtime=0;
        new_process.remaining_bursttime=new_process.total_bursttime;
        ready_queue[*queue_cnt]=new_process;
        ++(*queue_cnt);
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum){
    if(*queue_cnt == 0){return NULLPCB;}
    else{
        int earliest_arrival_index=0;
        int earliest_arrival = ready_queue[0].arrival_timestamp;
        for(int i =0;i<*queue_cnt;i++){
            if(ready_queue[i].arrival_timestamp<earliest_arrival){
                earliest_arrival_index=i;
                earliest_arrival = ready_queue[i].arrival_timestamp;
            }
        }
        struct PCB next = ready_queue[earliest_arrival_index];
        next.execution_starttime = time_stamp;
        next.execution_endtime = time_stamp+next.remaining_bursttime;
        for(int i = earliest_arrival_index;i<(*queue_cnt)-1;i++){
            ready_queue[i] = ready_queue[i+1];
        }
        --(*queue_cnt);
        return next;
    }
}