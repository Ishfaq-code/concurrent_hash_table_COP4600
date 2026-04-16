#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "database.h"
#include "hash.h"

void insert(hashTable *hash_table, const char* name, uint32_t salary, int priority, uint32_t hash){    
    hashRecord* curr = hash_table->head;
    hashRecord* prev = NULL;

    while(curr != NULL && curr->hash <= hash){
        if(curr->hash == hash){
            printf("Insert failed. Entry %u is a duplicate.\n", hash);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    hashRecord *new_record = (hashRecord *)malloc(sizeof(hashRecord));
    new_record->hash = hash;
    strcpy(new_record->name, name);
    new_record->salary = salary;
    
    if(prev){
        prev->next = new_record;
    }
    else{
        hash_table->head = new_record;
    }

    new_record->next = curr;
    printf("Inserted %u,%s,%u\n", new_record->hash, new_record->name, new_record->salary);
}

void delete(hashTable *hash_table, uint32_t hash){
    hashRecord* curr = hash_table->head;
    hashRecord* prev = NULL;

    while (curr != NULL && curr->hash < hash){
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL || curr->hash > hash){
        printf("Entry %u not deleted. Not in database.\n", hash);
        return;
    }

    if(prev){
        prev->next = curr->next;
    }
    else{
        hash_table->head = curr->next;
    }
    
    printf("Deleted record for %u,%s,%u\n", curr->hash, curr->name, curr->salary);
    free(curr);

}
