// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5009;

// Hash table
node *table[N];

//Counter to keep track of size of dictionary
unsigned int dict_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Find index by hashing word
    int index = hash(word);

    //If there is no linked list to check at index then return false for no match
    if (table[index] == NULL)
    {
        return false;
    }
    //Else check through linked list to check for match
    else
    {
        //loop through linked list to check for match
        for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
        {
            //compare word to word in dictionary if match return true
            if (strcasecmp(word, tmp->word) == 0)
            {
                return true;
            }
        }
    }
    //if no matches found return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //Initialize index as ANSII value for first letter in string
    int index = tolower(*word);

    //Move to next spot in memory to access next letter in string
    word++;

    //Until we reach end of string in memory, raise index by power of ANSII value of letter in string
    while(*word != '\0')
    {
        index ^= tolower(*word);

        //Move to next spot in memory allocated for characters of string
        word++;
    }

    //Return mod of index by N aka size of hash table
    return index%N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Open dictionary file
    FILE *file = fopen(dictionary, "r");

    //Check to make sure file opened without error
    if (!file)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    //Read strings from file
    //create a string to hold each word we are loading
    char *new_word = malloc(sizeof(char) * LENGTH);

    //Check to make sure malloc was able to allocate enough memory for node
    if (new_word == NULL)
    {
        return false;
    }

    while(fscanf(file, "%s", new_word) != EOF)
    {
        //Create a new node to store word in hash table
        node *new_node = malloc(sizeof(node));

        //Check to make sure malloc was able to allocate enough memory for node
        if (new_node == NULL)
        {
           return false;
        }

        //set word of node as new_word
        strcpy(new_node->word, new_word);

        //Set next pointer of node to NULL
        new_node->next = NULL;

        //Hash word to get index to use when inserting into hash table
        int index = hash(new_word);

        //if there is already list at index add to front of list
        if (table[index] != NULL){

            //set a temporary node pointing to what index is pointing to
            node *tmp = table[index];

            //set new node to point towards what index was pointing at
            new_node->next = tmp;

            //set index to point to new node now a beginning of linked list
            table[index] = new_node;
        }

        //Else set pointer in table to point to address of new_node
        else
        {
            table[index] = new_node;
        }

        //Keep track of how many words are in dictionary
        dict_size++;
    }

    //Free memory allocated through malloc
    free(new_word);

    //close dictionary file
    fclose(file);

    //Return true to show that dictionary was successfully loaded
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //Return global variable for dictionary size
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Iterate through table array to access addresses of head of lists
    for(int i = 0; i < N; i++)
    {
        //Check to make sure that index actually has a list to delete
        if (table[i] != NULL)
        {
            //set current node equal to address of head of list
            node *current = table[i];

            //Loop through linked list until you reach end
            while (current != NULL)
            {
                //set nxt equal to address of next node in list
                node *nxt = current->next;

                //free up the current node
                free(current);

                //set current equal to next node
                current = nxt;
            }

        }

    }
    return true;
}
