#ifndef CHASH_H
#define CHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rwlock.h"
#include "database.h"

// To enforce ordering between threads
typedef struct {
    pthread_mutex_t mut;
    pthread_cond_t cv;
    int next_priority;
} cv_ordering;

// Worker information that threads need
typedef struct {
    hashTable *table;
    char command[16];
    char name[100];
    int priority;
    uint32_t salary;
    rwlock_t *mutex;
    cv_ordering* order;
} WorkerArgs;

// Enumeration for different commands
enum COMMAND {
    INSERT = 0,
    SEARCH,
    UPDATE,
    DELETE,
    PRINT,
    INVALID
};

#endif