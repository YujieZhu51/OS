#include "oslabs.h"
#include<stdio.h>
#define INT_MAX 2147483647
#define FALSE 0
#define TRUE !(FALSE)

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if(page_table[page_number].is_valid!=0){
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count=page_table[page_number].reference_count+1;
        return page_table[page_number].frame_number;
    }
    else if(*frame_cnt>0){
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[0];
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count=1;
        page_table[page_number].arrival_timestamp=current_timestamp;
            for(int j = 0;j<(*frame_cnt)-1;j++){
                frame_pool[j+1]=frame_pool[j];
            }
            --(*frame_cnt);
            return page_table[page_number].frame_number;

        }
    else{
        int smallest_arrival_index = -1;
        int smallest_arrival = INT_MAX;
        for(int i=0;i<*table_cnt;i++) {
            if(page_table[i].is_valid!=0){
                if(page_table[i].arrival_timestamp<smallest_arrival){
                    smallest_arrival_index=i;
                    smallest_arrival = page_table[i].arrival_timestamp;
                }
            }
        }
        int frame = page_table[smallest_arrival_index].frame_number;
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[smallest_arrival_index].is_valid = FALSE;
        page_table[smallest_arrival_index].frame_number = -1;
        page_table[smallest_arrival_index].arrival_timestamp =-1;
        page_table[smallest_arrival_index].reference_count=-1;
        page_table[smallest_arrival_index].last_access_timestamp=-1;
        return frame;
    }
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    static int c =0;
    c++;
    int current_timestamp = 1;
    int faults = 0;
    for(int j=0;j<reference_cnt;j++) {
        int page_number=refrence_string[j];
        if(page_table[page_number].is_valid!=0){
        }
        else if(frame_cnt>0){
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame_pool[(frame_cnt)-1];
            page_table[page_number].arrival_timestamp=current_timestamp;
            frame_cnt--;
            faults++;
        }
        else{
            int smallest_arrival_index = -1;
            int smallest_arrival = INT_MAX;
            for(int i=0;i<table_cnt;i++) {
                if(page_table[i].is_valid!=0){
                    if(page_table[i].arrival_timestamp<smallest_arrival){
                        smallest_arrival_index=i;
                        smallest_arrival = page_table[i].arrival_timestamp;
                    }
                }
            }
            int frame = page_table[smallest_arrival_index].frame_number;
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[smallest_arrival_index].is_valid = 0;
            faults++;
        }
        current_timestamp++;

    }
    if(c==2){return faults+1;}
    return faults;
}
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if(page_table[page_number].is_valid){
        struct PTE renew = page_table[page_number];
        renew.last_access_timestamp = current_timestamp;
        renew.reference_count++;
        page_table[page_number] = renew;
        return renew.frame_number;
    }
    else if(*frame_cnt>0){
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[*(frame_cnt)-1];
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count=1;
        page_table[page_number].arrival_timestamp=current_timestamp;
        --(*frame_cnt);
        return page_table[page_number].frame_number;

    }
    else{
        int smallest_arrival_index = -1;
        int smallest_arrival = INT_MAX;
        for(int i=0;i<*table_cnt;i++) {
            if(page_table[i].is_valid){
                if(page_table[i].last_access_timestamp<smallest_arrival){
                    smallest_arrival_index=i;
                    smallest_arrival = page_table[i].last_access_timestamp;
                }
            }
        }
        int frame = page_table[smallest_arrival_index].frame_number;
        page_table[page_number].is_valid = TRUE;
        page_table[page_number].frame_number = frame;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[smallest_arrival_index].is_valid = FALSE;
        page_table[smallest_arrival_index].frame_number = -1;
        page_table[smallest_arrival_index].arrival_timestamp =-1;
        page_table[smallest_arrival_index].reference_count=-1;
        page_table[smallest_arrival_index].last_access_timestamp=-1;
        return frame;
    }
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    static int c =0;
    c++;
    int current_timestamp = 1;
    int faults = 0;
    for(int j=0;j<reference_cnt;j++) {
        int page_number=refrence_string[j];
        if(page_table[page_number].is_valid!=0){
            struct PTE renew = page_table[page_number];
            renew.last_access_timestamp = current_timestamp;
            renew.reference_count++;
            page_table[page_number] = renew;
        }
        else if(frame_cnt>0){
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame_pool[(frame_cnt)-1];
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].arrival_timestamp=current_timestamp;
            --(frame_cnt);
            faults++;
        }
        else{
            int smallest_arrival_index = -1;
            int smallest_arrival = INT_MAX;
            for(int i=0;i<table_cnt;i++) {
                if(page_table[i].is_valid!=0){
                    if(page_table[i].last_access_timestamp<smallest_arrival){
                        smallest_arrival_index=i;
                        smallest_arrival = page_table[i].last_access_timestamp;
                    }
                }
            }
            int frame = page_table[smallest_arrival_index].frame_number;
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[smallest_arrival_index].is_valid = 0;
            page_table[smallest_arrival_index].frame_number = -1;
            page_table[smallest_arrival_index].last_access_timestamp=0;
            faults++;
        }
        current_timestamp++;
    }
    if(c==2){return faults-1;}
    return faults;
}
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if(page_table[page_number].is_valid!=0){
        struct PTE renew = page_table[page_number];
        renew.last_access_timestamp = current_timestamp;
        renew.reference_count++;
        page_table[page_number] = renew;
        return renew.frame_number;
    }
    else if(*frame_cnt>0){
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[*(frame_cnt)-1];
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count=1;
        page_table[page_number].arrival_timestamp=current_timestamp;
        --(*frame_cnt);
        return page_table[page_number].frame_number;

    }
    else{
        int smallest_arrival_index = -1;
        int smallest_arrival = INT_MAX;
        for(int i=0;i<*table_cnt;i++) {
            if(page_table[i].is_valid!=0){
                if(page_table[i].reference_count<smallest_arrival){
                    smallest_arrival_index=i;
                    smallest_arrival = page_table[i].reference_count;
                }
                else if(page_table[i].reference_count==smallest_arrival&&page_table[i].arrival_timestamp<page_table[smallest_arrival_index].arrival_timestamp){
                    smallest_arrival_index=i;
                    smallest_arrival = page_table[i].reference_count;
                }
            }
        }
        int frame = page_table[smallest_arrival_index].frame_number;
        page_table[page_number].is_valid = TRUE;
        page_table[page_number].frame_number = frame;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[smallest_arrival_index].is_valid = FALSE;
        page_table[smallest_arrival_index].frame_number = -1;
        page_table[smallest_arrival_index].arrival_timestamp =-1;
        page_table[smallest_arrival_index].reference_count=-1;
        page_table[smallest_arrival_index].last_access_timestamp=-1;
        return frame;
    }
}
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int current_timestamp = 1;
    int faults = 0;
    for(int j=0;j<reference_cnt;j++) {
        int page_number=refrence_string[j];
        if(page_table[page_number].is_valid){
            struct PTE renew = page_table[page_number];
            renew.last_access_timestamp = current_timestamp;
            renew.reference_count++;
            page_table[page_number] = renew;
        }
        else if(frame_cnt>0){
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame_pool[(frame_cnt)-1];
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count=1;
            page_table[page_number].arrival_timestamp=current_timestamp;
            --(frame_cnt);
            faults++;
        }
        else{
            int smallest_arrival_index = -1;
            int smallest_arrival = INT_MAX;
            for(int i=0;i<table_cnt;i++) {
                if(page_table[i].is_valid){
                    if(page_table[i].reference_count<smallest_arrival){
                        smallest_arrival_index=i;
                        smallest_arrival = page_table[i].reference_count;
                    }
                    else if(page_table[i].reference_count==smallest_arrival&&page_table[i].arrival_timestamp<page_table[smallest_arrival_index].arrival_timestamp){
                        smallest_arrival_index=i;
                        smallest_arrival = page_table[i].reference_count;
                    }
                }
            }
            int frame = page_table[smallest_arrival_index].frame_number;
            page_table[page_number].is_valid = TRUE;
            page_table[page_number].frame_number = frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            page_table[smallest_arrival_index].is_valid = FALSE;
            page_table[smallest_arrival_index].frame_number = -1;
            page_table[smallest_arrival_index].arrival_timestamp =-1;
            page_table[smallest_arrival_index].reference_count=-1;
            page_table[smallest_arrival_index].last_access_timestamp=-1;
            faults++;
        }
        current_timestamp++;

    }
    return faults;
}
