/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Gaetan Ingrassia
 * Date: 2019-03-11
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    // FIXME: implement
    assert(map != 0);

    struct HashLink* cur = 0;
    struct HashLink* temp = 0;

    // Iterate through non-empty buckets only, stop when all elements freed
    int elemCounter = 0;
    int bucketCounter = 0;
    while (elemCounter < map->size)
    {
        if (map->table[bucketCounter] != 0)
        {
            cur = map->table[bucketCounter];
            while (cur != 0)
            {
                temp = cur->next;
                hashLinkDelete(cur);
                cur = temp;
                elemCounter++;
            }
            map->table[bucketCounter] = 0;
        }
        bucketCounter++;
    }

    map->size = 0;
    map->capacity = 0;
    free(map->table);
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    // FIXME: implement
    assert(map != 0);

    // Get index
    int index = HASH_FUNCTION(key) % map->capacity;
    // If negative index, make it in range
    if (index < 0)
        index += map->capacity;

    struct HashLink* cur = map->table[index];

    // Iterate through bucket, if key is found return address of value
    while (cur != 0)
    {
        if (strcmp(key, cur->key) == 0)
            return &(cur->value);
        cur = cur->next;
    }

    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity (double of the old capacity). After allocating the new table,
 * all of the links need to rehashed into it because the capacity has changed.
 *
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 *
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    // FIXME: implement

    assert(map != 0);

    // Create a new temporary map
    struct HashMap* newMap = hashMapNew(capacity);
    struct HashLink* cur = 0;
    struct HashLink* temp = 0;

    // Iterate through non-empty buckets only, re-hash them to the
    // new map and free the old links
    int elemCounter = 0;
    int bucketCounter = 0;
    while (elemCounter < map->size)
    {
        if (map->table[bucketCounter] != 0)
        {
            cur = map->table[bucketCounter];
            while (cur != 0)
            {
                hashMapPut(newMap, cur->key, cur->value);
                temp = cur->next;
                hashLinkDelete(cur);
                cur = temp;
                elemCounter++;
            }
        }
        bucketCounter++;
    }

    // Free the previous table and assign new one
    free(map->table);
    map->table = newMap->table;
    map->capacity = capacity;
    free(newMap);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 *
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    // FIXME: implement

    assert(map != 0);

    // Resize table if max load is reached
    if (hashMapTableLoad(map) >= MAX_TABLE_LOAD)
    {
        resizeTable(map, (2 * map->capacity));
    }

    // Get index
    int index = HASH_FUNCTION(key) % map->capacity;
    // If negative index, make it in range
    if (index < 0)
        index += map->capacity;

    // If the bucket is empty, create and place link
    if (map->table[index] == 0)
    {
        struct HashLink* link = hashLinkNew(key, value, 0);
        assert (link != 0);
        map->table[index] = link;
        map->size = map->size + 1;
    }
    else
    {
        struct HashLink* cur = map->table[index];
        struct HashLink* temp = cur;

        // Iterate until either end of linked list or match is found
        while (cur != 0 && strcmp(key, cur->key) != 0)
        {
            temp = cur;
            cur = cur->next;
        }

        // If no match, create and place link
        if (cur == 0)
        {
            struct HashLink* link = hashLinkNew(key, value, 0);
            assert (link != 0);
            temp->next = link;
            map->size = map->size + 1;//+;
        }
        // Otherwise, update matching value
        else
            cur->value = value;
    }
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    // FIXME: implement

    assert(map != 0);

    // Get index
    int index = HASH_FUNCTION(key) % map->capacity;
    // If negative index, make it in range
    if (index < 0)
        index += map->capacity;

    struct HashLink* cur = map->table[index];

    // If no links in bucket, do nothing
    if (cur == 0)
        return;

    // If the first element in the bucket is a match.
    // free it, assign new next, decrease map size
    if (strcmp(cur->key, key) == 0)
    {
        map->table[index] = map->table[index]->next;
        hashLinkDelete(cur);
        cur = 0;
        map->size = map->size - 1;
    }
    // Otherwise, iterate through and free a link if a match is found
    else
    {
        struct HashLink* temp = cur;

        while (cur != 0 && strcmp(key, cur->key) != 0)
        {
            temp = cur;
            cur = cur->next;
        }
        if (cur != 0)
        {
            temp->next = cur->next;
            hashLinkDelete(cur);
            map->size = map->size - 1;
        }
    }
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXME: implement

    assert(map != 0);

    // Get index
    int index = HASH_FUNCTION(key) % map->capacity;
    // Make within range if negative
    if (index < 0)
        index += map->capacity;

    struct HashLink* cur = map->table[index];

    // Return one if found within bucket, zero otherwise
    while (cur != 0)
    {
        if (strcmp(key, cur->key) == 0)
            return 1;
        cur = cur->next;
    }

    return 0;
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    // FIXME: implement
    return map->size;
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    // FIXME: implement
    assert(map != 0);

    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    // FIXME: implement
    assert(map != 0);

    int counter = 0;

    // Count buckets with a NULL pointer
    for (int i = 0; i < map->capacity; i++)
    {
        if (map->table[i] == 0)
            counter++;
    }

    return counter;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    // FIXME: implement
    assert(map != 0);

    float load = (float)(map->size) / map->capacity;

    return load;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
  // FIXME: implement
    assert(map != 0);

    // If the map is empty, tell user
    if (map->size == 0)
        printf("The map is empty.\n");
    // Otherwise, iterate through non-empty elements and print the
    // element number, key, and value
    else
    {
        struct HashLink* cur = 0;

        int elemCounter = 0;
        int bucketCounter = 0;
        while (elemCounter < map->size)
        {
            if (map->table[bucketCounter] != 0)
            {
                printf("Bucket %i -> ", bucketCounter);
                cur = map->table[bucketCounter];
                while (cur != 0)
                {
                    printf("(%s, %i) -> ", cur->key, cur->value);
                    cur = cur->next;
                    elemCounter++;
                }
                printf("\n");
            }
            bucketCounter++;
        }
    }
}
