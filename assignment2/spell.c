/* ... */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "spell.h"
#include "hash.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define TOTAL_ALPHA 26

/************************reused functino prototype ***************************/

int edit_distance(char *word1, char*word2);
int length_distance(char* a, char* b);

/**********************reused functino prototype end *************************/

// see Assignment Task 1: Computing edit distance
// As the the editing distance function might be resue later,
// it has been implemented as a individual reused fucntion
void print_edit_distance(char *word1, char *word2) {
	// print out the result
	printf("%d\n", edit_distance(word1, word2));
}

// see Assignment Task 2: Enumerating all possible edits
void print_all_edits(char *word) {
	// hold the word after modification
	char new[strlen(word)];
	// one more slot is necessary for implementing insertion
	char inser[strlen(word)+1];

	// implementing only deletion for 1 edit distance
	for (int i = 0; i < strlen(word); i++) {
		strcpy(new, word);
		// remove the target char in a string
		memmove(&new[i], &new[i + 1], strlen(new) - i);
		printf("%s\n", new);
	}

	// implementing only replacement for 1 edit distance
	for (int i = 0; i < strlen(word); i++) {
		strcpy(new, word);
		// replace each char in string with all 26 letter
		for (int j = 0; j < TOTAL_ALPHA; j++) {
			new[i] = (char)('a' + j);
			printf("%s\n", new);
		}
	}

	// implementing only insertion for 1 edit distance
	for (int i = 0; i < strlen(word)+1; i++) {
		strcpy(inser, word);
		memmove(&inser[i + 1], &inser[i], strlen(inser) - i + 1);

		for (int j = 0; j < TOTAL_ALPHA; j++) {
			inser[i] = (char)('a' + j);
			printf("%s\n", inser);
		}
	}
}


// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document) {
	// terminate if any of the list is NULL
	if(dictionary == NULL || document == NULL){
		return;
	}

	int size = ((dictionary->size) / 0.7);	// expected size of the hash table
	HashTable *table = new_hash_table(size); // creat table
	Node *item = dictionary->head;	// hold items in linked list
	int i=0;	// record the position of a word in dictionary

	// hash table initialization
	while(item != NULL){
		hash_table_put(table, (char*)(item->data), i);
		i++;
		item = item->next;
	}

	item = document->head;

	// implement the spelling check
	while(item != NULL){
		if(hash_table_has(table, (char*)(item->data))){
			printf("%s\n", (char*)(item->data));
		}else{
			printf("%s?\n", (char*)(item->data));
		}
		item = item->next;
	}

	free_hash_table(table);
}


// see Assignment Task 4: Spelling correction
void print_corrected(List *dictionary, List *document) {
	// terminate if any of the list is NULL
	if(dictionary == NULL || document == NULL){
		return;
	}

	int size = ((dictionary->size) / 0.7);	// expected size of the hash table
	HashTable *table = new_hash_table(size);	// creat table
	Node *item = dictionary->head;	// hold items in linked list
	int i=0;	// record the position of a word in dictionary

	//initialize the hash table
	while(item != NULL){
		hash_table_put(table, (char*)(item->data), i);
		i++;
		item = item->next;
	}

	item = document->head;

	// do spelling correct for each word
	while(item != NULL){
		if(hash_table_has(table, (char*)(item->data))){
			printf("%s\n", (char*)(item->data));
		}else{
			int position = -1;	// the position of a corrected in dictionary
			char *curr = (char*)(item->data);	// current processing word
			char corrected[strlen(curr)+1];	// store corrected word

			char new[strlen(curr)];	// store the processed word for each step

			// Deletion for 1 edit distance
			for (int i = 0; i < strlen(curr); i++) {
				strcpy(new, curr);
				// implement only remove the target char in a string
				memmove(&new[i], &new[i + 1], strlen(new) - i);
				if(hash_table_has(table, new)){
					if(position == -1){
						position = hash_table_get(table, new);
						strcpy(corrected, new);
					}else if(hash_table_get(table, new) < position){
						position = hash_table_get(table, new);
						strcpy(corrected, new);
					}
				}
			}

			// implement only replacement for 1 edit distance
			for (int i = 0; i < strlen(curr); i++) {
				strcpy(new, curr);
				// replace each char in string with all 26 letter
				for (int j = 0; j < TOTAL_ALPHA; j++) {
					new[i] = (char)('a' + j);

					if(hash_table_has(table, new)){
						if(position == -1){
							position = hash_table_get(table, new);
							strcpy(corrected, new);
						}else if(hash_table_get(table, new) < position){
							position = hash_table_get(table, new);
							strcpy(corrected, new);
						}
					}
				}
			}

			char inser[strlen(curr)+1];

			// implementing only insertion for 1 edit distance
			for (int i = 0; i < strlen(curr)+1; i++) {
				strcpy(inser, curr);
				memmove(&inser[i + 1], &inser[i], strlen(inser) - i + 1);

				for (int j = 0; j < TOTAL_ALPHA; j++) {
					inser[i] = (char)('a' + j);

					if(hash_table_has(table, inser)){
						if(position == -1){
							position = hash_table_get(table, inser);
							strcpy(corrected, inser);
						}else if(hash_table_get(table, inser) < position){
							position = hash_table_get(table, inser);
							strcpy(corrected, inser);
						}
					}
				}
			}

			if(position != -1){
				printf("%s\n", corrected);
				item = item->next;
				continue;
			}

			Node *dic_item = dictionary->head;
			int ed;

			while(dic_item != NULL){
				char* word_in_dic = (char*)(dic_item->data);
				int dis = length_distance(word_in_dic, curr);
				if(dis <= 3){
					ed = edit_distance(word_in_dic, curr);
					if(ed == 2){
						printf("%s\n", word_in_dic);
						break;
					}else if(ed == 3){
						if(position == -1){
							position = hash_table_get(table, word_in_dic);
							strcpy(corrected, word_in_dic);
						}
					}
				}
				dic_item = dic_item->next;
			}

			if(position != -1 && ed != 2){
				printf("%s\n", corrected);
			}else if (position == -1){
				printf("%s?\n", curr);
			}
			
		}

		item = item->next;
	}

	free_hash_table(table);
}

/************************reused fucntion**********************************/

int edit_distance(char *word1, char*word2){
	// initialization for the length of two words, +1 for the extra empty string
	int len1 = strlen(word1)+1, len2 = strlen(word2)+1;
	// generate a edit distance matrix for the two words
	int ** matrix = malloc(len1*sizeof(*matrix));

	// malloc for each slot int matrix for char in word
	for (int i = 0; i < len1; i++) {
		matrix[i] = malloc(len2*sizeof(**matrix));
		// initialize the first row for the empty string
		matrix[i][0] = i;
	}

	// initialize the first column for the empty string
	for (int i = 0; i < len2; i++) {
		matrix[0][i] = i;
	}

	// processing dynamic programming algorithms to find edit distance
	for (int i = 1; i < len1; i++) {
		for (int j = 1; j < len2; j++) {
			if(word2[j-1] == word1[i-1]){
				matrix[i][j] = MIN(MIN(matrix[i-1][j-1], matrix[i-1][j]+1), matrix[i][j-1]+1);
			}else{
				matrix[i][j] = MIN(MIN(matrix[i-1][j-1]+1, matrix[i-1][j]+1), matrix[i][j-1]+1);
			}
		}
	}

	int distance = matrix[len1-1][len2-1];

	// free the matrix
	for (int i = 0; i < len1; i++) {
		free(matrix[i]);
	}

	free(matrix);

	return distance;
}

int length_distance(char* a, char* b){
	int dis = strlen(a) - strlen(b);

	if(dis > 0){
		return dis;
	}else{
		return -dis;
	}
}

/************************reused function end **************************/
