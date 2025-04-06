//John Gitahi
//EUID: jgg0159
// CSCE 3600 -Major 2 - Alias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alias.h>



// ===============================================
// Hash Table for alias code
// ===============================================

// generate hash code
unsigned int hashCode(const char *key)
{
    unsigned int sum = 0;

    for (int i = 0; key[i]; i++)
    {
        sum += (unsigned char)key[i] * (i + 1);
    }
    return sum % MAX_HASH;
}

// free item
void freeItem(HashItem *item)
{
    free(item->key);
    free(item->value);
    free(item);
}

// function for deleting item from hash table
HashItem* getItem(HashItem *table[], const char *key)
{
    HashItem **link = &table[hashCode(key)];

    while (*link)
    {
        HashItem *tmp = *link;
        if (strcmp(tmp->key, key) == 0)
        {
            return tmp;
        }
        else
        {
            link = &(*link)->next;
        }
    }
}

// function for deleting item from hash table
void deleteItem(HashItem *table[], const char *key)
{
    HashItem **link = &table[hashCode(key)];

    while (*link)
    {
        HashItem *tmp = *link;
        if (strcmp(tmp->key, key) == 0)
        {
            *link = tmp->next; // unlink the list node
            freeItem(tmp);
            break;
        }
        else
        {
            link = &(*link)->next;
        }
    }
}

// free Table
void freeTable(HashItem *table[])
{
    for (int i = 0; i < MAX_HASH; i++)
    {
        for (HashItem *tmp = table[i]; tmp; tmp = tmp->next)
        {
            deleteItem(table, tmp->key);
        }
    }
}

// function for inserting item into the table
void insertItem(HashItem *table[], const char *key, const char *value)
{
    unsigned int code = hashCode(key);
    HashItem *item = malloc(sizeof(*item));
    if (item != NULL)
    {
        item->key = strdup(key);
        item->value = strdup(value);
        item->next = table[code];
        table[code] = item;
    }
}

// displaying items
void displayHashTable(HashItem *table[])
{
    printf("Current List: \n");
    for (int i = 0; i < MAX_HASH; i++)
    {
        for (HashItem *tmp = table[i]; tmp; tmp = tmp->next)
        {
            printf("- %s=%s\n", tmp->key, tmp->value);
        }
    }
}



