#include "oslabs.h"
#define INT_MAX 2147483647
struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    int fittest_index=-1;
    int fittest_size=INT_MAX;
    for(int i=0;i<*map_cnt;i++){
        if(memory_map[i].segment_size>=request_size&&memory_map[i].process_id==0){
            int distance = memory_map[i].segment_size-request_size;
            if(distance<fittest_size){
                fittest_size = distance;
                fittest_index = i;
            }
        }
    }
    if(fittest_index==-1){
        return NULLBLOCK;
    }
    else{
        struct MEMORY_BLOCK best_block = memory_map[fittest_index];
        if(fittest_size ==0){
            best_block.process_id = process_id;
            memory_map[fittest_index] = best_block;
            return best_block;
        }
        else{
            struct MEMORY_BLOCK b1 = {best_block.start_address,best_block.start_address+request_size-1,request_size,process_id};
            struct MEMORY_BLOCK b2 = {best_block.start_address+request_size, best_block.end_address,best_block.segment_size-request_size,0};
            memory_map[fittest_index] = b1;
            memory_map[*map_cnt]=b2;
            ++(*map_cnt);
            return b1;
        }

    }
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    for(int i=0;i<*map_cnt;i++){
        if(memory_map[i].segment_size>=request_size&&memory_map[i].process_id==0){
            int distance = memory_map[i].segment_size - request_size;
            struct MEMORY_BLOCK best_block = memory_map[i];
            if(distance == 0){
                best_block.process_id=process_id;
                memory_map[i] = best_block;
                return best_block;
            }
            else{
                struct MEMORY_BLOCK b1 = {best_block.start_address,best_block.start_address+request_size-1,request_size,process_id};
                struct MEMORY_BLOCK b2 = {best_block.start_address+request_size, best_block.end_address,best_block.segment_size-request_size,0};
                memory_map[i] = b1;
                memory_map[*map_cnt]=b2;
                ++(*map_cnt);
                return b1;
            }
        }
    }
    return NULLBLOCK;
}

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    int fittest_index=-1;
    int fittest_size=-1;
    for(int i=0;i<*map_cnt;i++){
        if(memory_map[i].segment_size>=request_size&&memory_map[i].process_id==0){
            int distance = memory_map[i].segment_size-request_size;
            if(distance>fittest_size){
                fittest_size = distance;
                fittest_index = i;
            }
        }
    }
    if(fittest_index==-1){
        return NULLBLOCK;
    }
    else{
        struct MEMORY_BLOCK best_block = memory_map[fittest_index];
        if(fittest_size ==0){
            best_block.process_id = process_id;
            memory_map[fittest_index] = best_block;
            return best_block;
        }
        else{
            struct MEMORY_BLOCK b1 = {best_block.start_address,best_block.start_address+request_size-1,request_size,process_id};
            struct MEMORY_BLOCK b2 = {best_block.start_address+request_size, best_block.end_address,best_block.segment_size-request_size,0};
            memory_map[fittest_index] = b1;
            memory_map[*map_cnt]=b2;
            ++(*map_cnt);
            return b1;
        }

    }
}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK   memory_map[MAPMAX],int *map_cnt, int process_id, int last_address){
    int shortest_start = INT_MAX;
    int shortest_start_index = -1;
    for(int i=0;i<*map_cnt;i++) {
        if (memory_map[i].start_address>=last_address&&memory_map[i].segment_size >= request_size&&memory_map[i].process_id==0) {
            int distance = memory_map[i].start_address-last_address;
            if(distance<shortest_start){
                shortest_start = distance;
                shortest_start_index = i;
            }
        }
    }
    if(shortest_start_index ==-1){return NULLBLOCK;}
    else{
        struct MEMORY_BLOCK best_block = memory_map[shortest_start_index];
        if(best_block.segment_size ==request_size){
            best_block.process_id = process_id;
            memory_map[shortest_start_index] = best_block;
            return best_block;
        }
        else{
            struct MEMORY_BLOCK b1 = {best_block.start_address,best_block.start_address+request_size-1,request_size,process_id};
            struct MEMORY_BLOCK b2 = {best_block.start_address+request_size, best_block.end_address,best_block.segment_size-request_size,0};
            memory_map[shortest_start_index] = b1;
            memory_map[*map_cnt]=b2;
            ++(*map_cnt);
            return b1;
        }
    }
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt){
    int left_index = -1;
    int right_index = -1;
    int this_index =-1;
    for(int i=0;i<*map_cnt;i++) {
        if(memory_map[i].start_address == freed_block.end_address+1&&memory_map[i].process_id==0){
            right_index = i;
        }
        if(memory_map[i].end_address == freed_block.start_address-1&&memory_map[i].process_id==0){
            left_index = i;
        }
        if(memory_map[i].start_address == freed_block.start_address){
            this_index = i;
        }
    }
    if(left_index==-1 && right_index ==-1){
        freed_block.process_id = 0;
        memory_map[*map_cnt]=freed_block;
        ++(*map_cnt);
    }
    else if(left_index !=-1&& right_index!=-1){
        struct MEMORY_BLOCK combined = {memory_map[left_index].start_address,memory_map[right_index].end_address,memory_map[left_index].segment_size+memory_map[right_index].segment_size+freed_block.segment_size,0};
        int max, min;
        if(left_index>right_index){max = left_index;min =right_index;}
        else{max =right_index;min = left_index;}
        memory_map[min] = combined;
        for(int i = max;i<(*map_cnt)-1;i++){
            memory_map[i] = memory_map[i+1];
        }
        --(*map_cnt);
    }
    else if(left_index!=-1){
        struct MEMORY_BLOCK combined = {memory_map[left_index].start_address,freed_block.end_address,memory_map[left_index].segment_size+freed_block.segment_size,0};
        memory_map[left_index] = combined;
    }
    else{
        struct MEMORY_BLOCK combined = {freed_block.start_address,memory_map[right_index].end_address,memory_map[right_index].segment_size+freed_block.segment_size,0};
        memory_map[right_index] = combined;
    }
    for(int i =this_index;i<(*map_cnt)-1;i++){
        memory_map[i] = memory_map[i+1];
    }
    --(*map_cnt);

}

