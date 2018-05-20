#ifndef HASH_H
#define HASH_H

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// this is an item in a hash table
typedef struct bucket Bucket;
struct bucket {
    char* key;  // correct spelling word in dictionary
    int value;  // the position of the word in linked list in dictionary
    Bucket *next; // using seperate chaining to deal with the collision
};

// hash table itself
typedef struct hash_table HashTable;
struct hash_table{
    int size;   // number of slot in the hash table
    Bucket** buckets;   //  a pointer point to the table
};

HashTable *new_hash_table(int size);
void free_hash_table(HashTable *table);

void hash_table_put(HashTable *table, char *key, int value);
int  hash_table_get(HashTable *table, char *key);
bool hash_table_has(HashTable *table, char *key);

#endif
