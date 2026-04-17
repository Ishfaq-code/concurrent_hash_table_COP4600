#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define COMMANDS "commands.txt"
#define MAX_LINE 256
#define LOG "hash.log"

#include "database.h"
#include "hash.h"
#include "rwlock.h"
#include "common.h"
#include "chash.h"


FILE* lp;

void write_to_log(enum COMMAND cmd, WorkerArgs *worker, uint32_t hash){
    switch (cmd)
    {
    case 0:
        fprintf(lp, "%lld Thread: %d INSERT, %u,%s,%u\n", current_timestamp(), worker->priority, hash, worker->name, worker->salary);
        break;
    case 1:
        fprintf(lp, "%lld Thread: %d SEARCH, %u,%s\n", current_timestamp(), worker->priority, hash, worker->name);
        break;
    case 2:
        fprintf(lp, "%lld Thread: %d UPDATE, %u,%s,%u\n", current_timestamp(), worker->priority, hash, worker->name, worker->salary);
        break;
    case 3:
        fprintf(lp, "%lld Thread: %d DELETE, %u,%s\n", current_timestamp(), worker->priority, hash, worker->name);
        break;
    case 4:
        fprintf(lp, "%lld Thread: %d PRINT\n", current_timestamp(), worker->priority);
        break;
    default:
        break;
    }
}

void execute_hash(enum COMMAND cmd, WorkerArgs *worker,uint32_t hash, lock_fn acquire_lock, lock_fn release_lock, const char *lock_label){
    write_to_log(cmd, worker, hash);
    acquire_lock(worker->mutex);
    fprintf(lp, "%lld: Thread %d %s LOCK ACQUIRED\n", current_timestamp(), worker->priority, lock_label);
    switch (cmd)
    {
    case 0:
        insert(worker->table, worker->name, worker->salary, worker->priority, hash);
        break;
    case 1:
        search(worker->table, hash, worker->name);
        break;
    case 2:
        update(worker->table, hash, worker->salary);
        break;
    case 3:
        delete(worker->table, hash);
        break;
    case 4:
        print_table(worker->table);
        break;
    default:
        break;
    }
    release_lock(worker->mutex);
    fprintf(lp, "%lld: Thread %d %s LOCK RELEASED\n", current_timestamp(), worker->priority, lock_label);
}

void interpret_command(char* raw_command, WorkerArgs *worker,uint32_t hash){
    if (strcmp(raw_command, "insert") == 0) {
        execute_hash(INSERT, worker, hash, rwlock_acquire_writelock, rwlock_release_writelock, "WRITE");
    }
    else if (strcmp(raw_command, "search") == 0) {
        execute_hash(SEARCH, worker, hash, rwlock_acquire_readlock, rwlock_release_readlock, "READ");
    }
    else if(strcmp(raw_command, "delete") == 0){
        execute_hash(DELETE, worker, hash, rwlock_acquire_writelock, rwlock_release_writelock, "WRITE");
    }
    else if(strcmp(raw_command, "update") == 0){
        execute_hash(UPDATE, worker, hash, rwlock_acquire_writelock, rwlock_release_writelock, "WRITE");
    }
    else if(strcmp(raw_command, "print") == 0){
        execute_hash(PRINT, worker, hash, rwlock_acquire_readlock, rwlock_release_readlock, "READ");
    }
}



void* worker(void* args){
    WorkerArgs *worker = (WorkerArgs *)args;

    fprintf(lp, "%lld: Thread %d WAITING FOR MY TURN\n", current_timestamp(), worker->priority);
    pthread_mutex_lock(&worker->order->mut);
    while (worker->priority != worker->order->next_priority) {
        pthread_cond_wait(&worker->order->cv, &worker->order->mut);
    }
    pthread_mutex_unlock(&worker->order->mut);
    fprintf(lp, "%lld: Thread %d AWAKENED FOR WORK\n", current_timestamp(), worker->priority);
    
    uint32_t hash = jenkins_one_at_a_time_hash(worker->name, strlen(worker->name));
    interpret_command(worker->command, worker, hash);

    pthread_mutex_lock(&worker->order->mut);
    worker->order->next_priority++;
    pthread_cond_broadcast(&worker->order->cv);
    pthread_mutex_unlock(&worker->order->mut);

    return NULL;

}

int main(){
    FILE *fp = fopen(COMMANDS, "r");
    if (fp == NULL) {
        perror("Failed to open commands.txt");
        return -1;
    }

    lp = fopen(LOG, "w");
    if (lp == NULL) {
        perror("Failed to open hash.log");
        return -1;
    }

    char line[MAX_LINE];
    pthread_t* threads = NULL;
    WorkerArgs *args = NULL;
    int thread_count = 0;

    // ---- Read first line separately ----
    if (fgets(line, sizeof(line), fp) != NULL) {
        char label[50];
        int parsed_thread_count;
        int first_id;

        // Example first line: threads,60,0
        if (sscanf(line, "%49[^,],%d,%d", label, &parsed_thread_count, &first_id) == 3) {
            thread_count = parsed_thread_count;
            threads = (pthread_t*)malloc(sizeof(pthread_t) * thread_count);
            args = (WorkerArgs *)malloc(sizeof(WorkerArgs) * thread_count);
            if (threads == NULL || args == NULL) {
                perror("Failed to allocate thread data");
                fclose(fp);
                free(threads);
                free(args);
                return -1;
            }
        }
    }

    if (threads == NULL || args == NULL || thread_count <= 0) {
        fprintf(stderr, "Invalid thread configuration in first line.\n");
        fclose(fp);
        free(threads);
        free(args);
        return -1;
    }

    rwlock_t mutex;
    rwlock_init(&mutex);

    cv_ordering order;
    order.next_priority = 0;
    pthread_mutex_init(&order.mut, NULL);
    pthread_cond_init(&order.cv, NULL);

    hashTable* table = (hashTable*)malloc(sizeof(hashTable));
    table->head = NULL;
    int counter = 0;

    // ---- Read remaining lines one by one ----
    while (fgets(line, sizeof(line), fp) != NULL) {
        char command[50];
        char name[100];
        int value;
        int id;

        // Example line: insert,Shigeru Miyamoto,85000,0
        if (sscanf(line, "%49[^,],%99[^,],%d,%d", command, name, &value, &id) == 4) {
            if (counter >= thread_count) {
                break;
            }

            args[counter].table = table;
            args[counter].mutex = &mutex;
            strncpy(args[counter].command, command, sizeof(args[counter].command) - 1);
            args[counter].command[sizeof(args[counter].command) - 1] = '\0';
            strncpy(args[counter].name, name, sizeof(args[counter].name) - 1);
            args[counter].name[sizeof(args[counter].name) - 1] = '\0';
            args[counter].priority = id;
            args[counter].salary = value;
            args[counter].order = &order;
            pthread_create(&threads[counter], NULL, worker, &args[counter]);
            counter++;
        }
    }

    for(int i = 0; i < counter; i++){
        pthread_join(threads[i], NULL);
    }
    pthread_cond_destroy(&order.cv);
    pthread_mutex_destroy(&order.mut); 



    print_table(table);


    fclose(fp);
    free(args);
    free(threads);
    return 0;
     
}


