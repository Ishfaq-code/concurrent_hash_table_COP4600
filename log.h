#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include "chash.h"

int init_log();
void notify_lock(const char* lock_type, int thread_id, char* action);
void notify_thread_action(enum COMMAND cmd, int thread_id, uint32_t hash, char* name, uint32_t salary);
void write_wait(int priority);
void write_awake(int priority);
void print_lock_count(int lock_count, int release_count);
void print_final_table(hashTable *hash_table);
void close_lp();

#endif