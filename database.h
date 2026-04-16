#ifndef DATABASE_H
#define DATABASE_H

#include <stdint.h>

typedef struct hash_struct {
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

typedef struct hash_table_struct {
    hashRecord* head;
} hashTable;

void insert(hashTable *hash_table, const char* name, uint32_t salary, int priority, uint32_t hash);
void delete(hashTable *hash_table, uint32_t hash);
void update(hashTable *hash_table, uint32_t hash, uint32_t salary);
void print_table(hashTable *hash_table);

#endif