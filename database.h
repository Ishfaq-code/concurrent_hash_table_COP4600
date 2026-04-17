#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdint.h>

// Struct for storing records/nodes
typedef struct hash_struct {
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

// Hash Table intialization
typedef struct hash_table_struct {
    hashRecord* head;
} hashTable;

// Insert a Value into a Hash table, fails if it already exists
void insert(hashTable *hash_table, const char* name, uint32_t salary, int priority, uint32_t hash);
// Delete a value from the table, fails if value does not exist
void delete(hashTable *hash_table, uint32_t hash, const char* name, uint32_t salary, int priority);
// Update a value that exists in the table
void update(hashTable *hash_table, uint32_t hash, const char* name, uint32_t salary, int priority);
// Print the entire table
void print_table(hashTable *hash_table, uint32_t hash, const char* name, uint32_t salary, int priority);
// Search for a value in the table
void search(hashTable *hash_table, uint32_t hash, const char* name, uint32_t salary, int priority);
// Write table to database
void write_table_log(FILE* lp, hashTable *hash_table, int priority);

#endif
