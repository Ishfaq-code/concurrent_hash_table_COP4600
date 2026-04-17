#ifndef DATABASE_H
#define DATABASE_H

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
void delete(hashTable *hash_table, uint32_t hash);
// Update a value that exists in the table
void update(hashTable *hash_table, uint32_t hash, uint32_t salary);
// Print the entire table
void print_table(hashTable *hash_table);
// Search for a value in the table
void search(hashTable *hash_table, uint32_t hash, const char* name);

#endif