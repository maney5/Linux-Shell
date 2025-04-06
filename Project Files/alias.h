//John Gitahi
//EUID: jgg0159
// CSCE 3600 -Major 2 - Alias.h
#ifndef APPEND_H_   
#define APPEND_H_

#define MAX_HASH 1027

typedef struct HashItem
{
    char *key;
    char *value;
    struct HashItem *next;
} HashItem;


// generate hash code
unsigned int hashCode(const char *key);
// free item
void freeItem(HashItem *item);

// function for deleting item from hash table
HashItem* getItem(HashItem *table[], const char *key);

// function for deleting item from hash table
void deleteItem(HashItem *table[], const char *key);

// free Table
void freeTable(HashItem *table[]);

// function for inserting item into the table
void insertItem(HashItem *table[], const char *key, const char *value);
// displaying items
void displayHashTable(HashItem *table[]);

#endif 