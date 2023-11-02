#include "oslabs.h"
#include <stdlib.h>
#define INT_MAX 2147483647
struct RCB NULLRCB = {0,0,0,0,0};
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    int sum = current_request.arrival_timestamp+current_request.process_id+current_request.address+current_request.cylinder+current_request.request_id;
    if(sum == 0){
        return new_request;
    }
    else{
        request_queue[*queue_cnt] = new_request;
        ++(*queue_cnt);
        return current_request;
    }
}
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
    if(*queue_cnt == 0){return NULLRCB;}
    else{
        int earliest_arrival_index=0;
        int earliest_arrival = request_queue[0].arrival_timestamp;
        for(int i = 0;i<*queue_cnt;i++){
            if(request_queue[i].arrival_timestamp<earliest_arrival){
                earliest_arrival_index=i;
                earliest_arrival = request_queue[i].arrival_timestamp;
            }
        }
        struct RCB next = request_queue[earliest_arrival_index];
        for(int i = earliest_arrival_index;i<(*queue_cnt)-1;i++){
            request_queue[i] = request_queue[i+1];
        }
        --(*queue_cnt);
        return next;
    }
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    int sum = current_request.arrival_timestamp+current_request.process_id+current_request.address+current_request.cylinder+current_request.request_id;
    if(sum == 0){
        return new_request;
    }
    else{
        request_queue[*queue_cnt] = new_request;
        ++(*queue_cnt);
        return current_request;
    }
}
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
    if(*queue_cnt == 0){return NULLRCB;}
    else{
        int shortest_distance_index=0;
        int shortest_distance = abs(request_queue[0].cylinder-current_cylinder);
        for(int i = 0;i<*queue_cnt;i++){
            int distance = abs(request_queue[i].cylinder-current_cylinder);
            if(distance<shortest_distance){
                shortest_distance_index=i;
                shortest_distance = distance;
            }
            else if(distance==shortest_distance){
                if(request_queue[i].arrival_timestamp<request_queue[shortest_distance_index].arrival_timestamp){
                    shortest_distance_index=i;
                }
            }
        }
        struct RCB next = request_queue[shortest_distance_index];
        for(int i = shortest_distance_index;i<(*queue_cnt)-1;i++){
            request_queue[i] = request_queue[i+1];
        }
        --(*queue_cnt);
        return next;
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    int sum = current_request.arrival_timestamp+current_request.process_id+current_request.address+current_request.cylinder+current_request.request_id;
    if(sum == 0){
        return new_request;
    }
    else{
        request_queue[*queue_cnt] = new_request;
        ++(*queue_cnt);
        return current_request;
    }
}
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction){
    if(*queue_cnt == 0){return NULLRCB;}
    else{
        int same_earliest_index=-1;
        int same_earliest=0;
        for(int i = 0;i<*queue_cnt;i++){
            if(request_queue[i].cylinder ==current_cylinder){
                if(same_earliest_index==-1){
                    same_earliest_index = i;
                    same_earliest = request_queue[i].arrival_timestamp;
                }
                else{
                    if(request_queue[i].arrival_timestamp<same_earliest){
                        same_earliest_index = i;
                        same_earliest = request_queue[i].arrival_timestamp;
                    }
                }
            }
        }
        if(same_earliest_index==-1){
            int biggest_index = -1;
            int smallest_index = -1;
            int biggest = INT_MAX;
            int smallest = INT_MAX;
            for(int i = 0;i<*queue_cnt;i++){
                int distance = abs(request_queue[i].cylinder-current_cylinder);
                if(request_queue[i].cylinder>current_cylinder){
                    if(distance<biggest){
                        biggest_index = i;
                        biggest = distance;
                    }
                }
                else{
                    if(distance<smallest){
                        smallest_index = i;
                        smallest = distance;
                    }
                }
            }
            if(scan_direction==1){
                if(biggest_index!=-1){
                    same_earliest_index =biggest_index;
                }
                else{same_earliest_index = smallest_index;}
            }
            else{
                if(smallest_index!=-1){
                    same_earliest_index =smallest_index;
                }
                else{same_earliest_index = biggest_index;}
            }
        }
        struct RCB next = request_queue[same_earliest_index];
        for(int i = same_earliest_index;i<(*queue_cnt)-1;i++){
            request_queue[i] = request_queue[i+1];
        }
        --(*queue_cnt);
        return next;
    }
}




