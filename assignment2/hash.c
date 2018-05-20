#include "hash.h"

/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia
 */
Bucket *new_bucket(char *key, int value) {
	Bucket *bucket = malloc(sizeof *bucket);
	assert(bucket);

	// create own copy of key for storage in table
	bucket->key = malloc((sizeof *bucket->key) * (strlen(key) + 1));
	assert(bucket->key);
	strcpy(bucket->key, key);

	bucket->value = value;
	bucket->next = NULL;

	return bucket;
}

/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia
 */
void free_bucket(Bucket *bucket) {
	assert(bucket != NULL);
	free(bucket->key);
	free(bucket);
}

struct table {
	int size;
	Bucket **buckets;
};

/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia
 */
HashTable *new_hash_table(int size) {
	HashTable *table = malloc(sizeof *table);
	assert(table);

	table->size = size;
	table->buckets = malloc(size * (sizeof *table->buckets));
	assert(table->buckets);
	int i;
	for (i = 0; i < size; i++) {
		table->buckets[i] = NULL;
	}

	return table;
}

/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia with a
 * little modification.
 */
void free_hash_table(HashTable *table) {
	assert(table != NULL);

	int i;
	for (i = 0; i < table->size; i++) {
		Bucket *this_bucket, *next_bucket;
		this_bucket = table->buckets[i];
		while (this_bucket) {
			next_bucket = this_bucket->next;
			free_bucket(this_bucket);
			this_bucket = next_bucket;
		}
	}

	free(table->buckets);
	free(table);
}

// hash function
int xorhash(char *key, int size){
    unsigned int h = size;

    for (int i = 0; i< strlen(key); i++) {
        //printf("i=%d\n", i);
        h = h ^ ((h << 5) + key[i] + (h >> 2));
    }

    //printf("pass\n");
    //printf("size=%d\n", size);
    //printf("h=%u\n",h);
    //printf("position=%u", h % size);
    //printf("position=%d", (int)(h % size));

    return (int)(h % size);
}

/*
 * hash helper fucntion
 */

bool equal(char *a, char *b){
    return strcmp(a,b) == 0;
}

/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia with a little modification
 */
 void hash_table_put(HashTable *table, char *key, int value) {
 	assert(table != NULL);
 	assert(key != NULL);

 	int hash_value = xorhash(key, table->size);
    //printf("hash_value=%d", hash_value);
 	// look for existing key
 	Bucket *bucket = table->buckets[hash_value];
 	while (bucket) {
 		if (equal(key, bucket->key)) {
 			bucket->value = value;
 			return;
 		}
 		bucket = bucket->next;
 	}

 	// if key wasn't found, add it at front of list
 	Bucket *new = new_bucket(key, value);
 	new->next = table->buckets[hash_value];
 	table->buckets[hash_value] = new;
 }


/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia
 */
int hash_table_get(HashTable *table, char *key) {
    assert(table != NULL);
    assert(key != NULL);

    int hash_value = xorhash(key, table->size);

  	// look for existing key
    Bucket *bucket = table->buckets[hash_value];
    while (bucket) {
        if (equal(key, bucket->key)) {
            return bucket->value;
        }
        bucket = bucket->next;
    }

    // key doesn't exist!
    fprintf(stderr, "error: key \"%s\" not found in table\n", key);
    exit(1);
}

/* attribute: this function refer to the code in workshop 9
 * int the file hashtbl.c created by Matt Farrugia
 */
bool hash_table_has(HashTable *table, char *key) {
    assert(table != NULL);
    assert(key != NULL);

    int hash_value = xorhash(key, table->size);

    // look for existing key
 	Bucket *bucket = table->buckets[hash_value];
 	while (bucket) {
 		if (equal(key, bucket->key)) {
 			return true;
 		}
 		bucket = bucket->next;
 	}

 	// key doesn't exist!
 	return false;
}
