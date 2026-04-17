#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "chash.h"
#include "database.h"

#define LOG "hash.log"


FILE* lp;

int init_log(){
    lp = fopen(LOG, "w");
    if (lp == NULL) {
        perror("Failed to open hash.log");
        return -1;
    }

    return 1;
}

void notify_lock(char* lock_type, int thread_id, char* action){
    fprintf(lp, "%lld: THREAD %d %s %s\n", current_timestamp(), thread_id, lock_type, action);
}

void notify_thread_action(enum COMMAND cmd, int thread_id, uint32_t hash, char* name, uint32_t salary){
    switch (cmd)
    {
    case 0:
        fprintf(lp, "%lld: THREAD %d INSERT, %u,%s,%u\n", current_timestamp(), thread_id, hash, name, salary);
        break;
    case 1:
        fprintf(lp, "%lld: THREAD %d SEARCH, %u,%s\n", current_timestamp(), thread_id, hash, name);
        break;
    case 2:
        fprintf(lp, "%lld: THREAD %d UPDATE, %u,%s,%u\n", current_timestamp(), thread_id, hash, name, salary);
        break;
    case 3:
        fprintf(lp, "%lld: THREAD %d DELETE, %u,%s\n", current_timestamp(), thread_id, hash, name);
        break;
    case 4:
        fprintf(lp, "%lld: THREAD %d PRINT\n", current_timestamp(), thread_id);
        break;
    default:
        break;
    }
}

void write_wait(int priority){
    fprintf(lp, "%lld: THREAD %d WAITING FOR MY TURN\n", current_timestamp(), priority);
}

void write_awake(int priority){
    fprintf(lp, "%lld: THREAD %d AWAKENED FOR WORK\n", current_timestamp(), priority);
}

void print_lock_count(int lock_count, int release_count){
    fprintf(lp, "\n");
    fprintf(lp, "Number of lock acquisitions: %d\n", lock_count);
    fprintf(lp, "Number of lock releases: %d\n", release_count);
}

void print_final_table(hashTable *hash_table){
    write_table_log(lp, hash_table, 0);
}

void close_lp(){
    fclose(lp);
}

