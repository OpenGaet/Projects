#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Suggested word structure, with current
// suggestion and its Levenshtein distance
struct Suggested
{
    char* sugWord;
    int dist;
};


/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    assert (map != 0);

    // Get the first word in the file
    char* key = nextWord(file);

    // Add words in file to map, until end of file,
    // storing their length as the value
    while (key != 0)
    {
        hashMapPut(map, key, strlen(key));
        free(key);
        key = nextWord(file);
    }
}


/**
 * Returns the smallest integer parameter.
 * @param integer
 * @param integer
 * @param integer
 */
int min(int num1, int num2, int num3)
{
    int min = num3;

    if (num1 < num3)
        min = num1;

    if (num2 < min)
        min = num2;

    return min;
}


/**
 * Returns the Levenshtein Distance of the two parameters.
 * @param string1
 * @param string1 length
 * @param string2
 * @param string2 length
 * Credit: https://en.wikipedia.org/wiki/Levenshtein_distance
 */
int levDist(char* word1, int L1, char* word2, int L2)
{

   //*

    // Two row implementation of Levenshtein's Algorithm
    int v0[L2 + 1];
    int v1[L2 + 1];
    int subCost, delCost, inCost;

    // Initialize row to [0...L2]
    for (int i = 0; i <= L2; i++)
        v0[i] = i;

    for (int i = 0; i < L1; i++)
    {
        v1[0] = i + 1;

        // Determine deletion, insertion, and substitution costs
        for (int j = 0; j < L2; j++)
        {
            delCost = v0[j + 1] + 1;
            inCost = v1[j] + 1;

            if (word1[i] == word2[j])
                subCost = v0[j];
            else
                subCost = v0[j] + 1;

            // Assign the lowest cost option
            v1[j + 1] = min(delCost, inCost, subCost);
        }
        // Copy v1 to v0
        memcpy(v0, v1, sizeof(v1));
    }

    return v0[L2];
//*/



/*  Iterative Full Matrix Implementation

    int subCost, delCost, inCost;

    int d[L1+1][L2+1];

    for (int i = 0; i <= L1; i++)
    {
        for (int j = 0; j <= L2; j++)
            d[i][j] = 0;
    }

    for (int i = 1; i <= L1; i++)
        d[i][0] = i;

    for (int j = 1; j <= L2; j++)
        d[0][j] = j;

    for (int j = 1; j <= L2; j++)
    {
        for (int i = 1; i <= L1; i++)
        {
            if (word1[i] == word2[j])
                subCost = 0;
            else
                subCost = 1;
            d[i][j] = min(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + subCost);
        }
    }

    return d[L1][L2];
*/

    /* Alternative Implementation with one row
    //
    int v0[L1+1];
    int subCost, delCost, inCost, result, oldResult;

    for (int i = 1; i <= L1; i++)
        v0[i] = i;

    for (int i = 1; i <= L2; i++)
    {
        v0[0] = i;


        // use formula to fill in the rest of the row
        for (int j = 1, result = i-1; j <= L1; j++)
        {
            oldResult = v0[j];
            delCost = v0[j] + 1;
            inCost = v0[j-1] + 1;

            if (word1[i-1] == word2[j-1])
                subCost = result;
            else
                subCost = result + 1;

            v0[j] = min(delCost, inCost, subCost);
            result = oldResult;
        }
    }

    return v0[L1];
    */
}


/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    HashMap* map = hashMapNew(1000);

    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);

        // Implement the spell checker code here..

        // Make sure only one word input
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        char tempBuffer[256];

        // Make case-insensitive
        for (int i = 0; inputBuffer[i] != '\0' || inputBuffer[i] == ' '; i++)
        {
            tempBuffer[i] = tolower(inputBuffer[i]);
            tempBuffer[i+1] = '\0';
        }

        // If correctly spelled, tell user
        if (hashMapContainsKey(map, tempBuffer))
            printf("The inputted word '%s' is spelled correctly.\n", tempBuffer);
        // Otherwise, iterate through the map, computing the Levenshtein distance to the given word
        else
        {
            // Maintain an ordered array of suggested words with lowest distance
            struct Suggested suggestions[5];
            for (int i = 0; i < 5; i++)
                suggestions[i].dist = 100;

            struct HashLink* cur = 0;
            int inLength = strlen(tempBuffer);
            int elemCounter = 0;
            int bucketCounter = 0;
            int editDist = 0;

            // Iterate through the map, computing the distance of each word to the input word
            while (elemCounter < map->size)
            {
                if (map->table[bucketCounter] != 0)
                {
                    cur = map->table[bucketCounter];
                    while (cur != 0)
                    {
                        editDist = levDist(cur->key, cur->value, tempBuffer, inLength);

                        // Iterate through suggested word array, starting with the smallest distance element
                        for (int i = 0; i < 5; i++)
                        {
                            // If current word has a smaller distance, place in array in order
                            if (editDist < suggestions[i].dist)
                            {
                                for (int j = 4; j > i; j--)
                                {
                                    suggestions[j].sugWord = suggestions[j-1].sugWord;
                                    suggestions[j].dist = suggestions[j-1].dist;
                                }
                                suggestions[i].sugWord = cur->key;
                                suggestions[i].dist = editDist;
                                break;
                            }
                        }
                        cur = cur->next;
                        elemCounter++;
                    }
                }
                bucketCounter++;
            }
            // Print suggested words
            printf("The inputted word '%s' is spelled incorrectly.\n", tempBuffer);
            printf("Did you mean: %s, %s, %s, %s, or %s?\n", suggestions[0].sugWord, suggestions[1].sugWord,
                                                            suggestions[2].sugWord, suggestions[3].sugWord, suggestions[4].sugWord);
        }

        if (strcmp(tempBuffer, "quit") == 0)
        {
            quit = 1;
        }
    }

    hashMapDelete(map);
    return 0;
}
