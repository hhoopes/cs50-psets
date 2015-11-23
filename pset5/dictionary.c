/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"


// define a trie node
typedef struct trie_node trie_node;
struct trie_node
{
    bool is_word;
    trie_node* children[ALPHABET];    
};

// define the trie structure
typedef struct trie trie;
struct trie
{
    trie_node* root;
    int wordcount;
};


// creating a new node
trie_node* createNode(void)
{
    trie_node* dNode = NULL;
 
    dNode = (trie_node*)malloc(sizeof(trie_node));
 
    if( dNode )
    {
        int i;
 
        dNode->is_word = false;
 
        for(i = 0; i < ALPHABET; i++)
        {
            dNode->children[i] = NULL;
        }
    }
 
    return dNode;
}

//deleting all nodes
void deleteChildren(trie_node* current)
{ 
    for (int i = 0; i < ALPHABET; i++)
    {               
        if (current->children[i] != NULL)
        { 
            deleteChildren(current->children[i]);
        }
    }
    
    free(current);
}

int lettermath(char letter)
{
    int letter_mod = 0;
    if ((isalpha(letter)) || (letter == '\''))
    {
        if (letter == '\'') letter_mod = 26;
            
        else letter_mod = toupper(letter) - 'A';
       
    }
    
    else letter_mod = -1;
    
    return letter_mod;
}    
               
//initialize the trie
 trie dTrie = {NULL, 0};


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    trie_node* current; // pointer for where you're at
    current = dTrie.root;
    
   
    for(int i = 0; word[i] != '\0'; i++)
    {        
        int letnum = lettermath(word[i]);
        
        // printf("You inputted %c and it came back %i\n", word[i], letnum);
    
            
        // not a leaf or another node, so it's misspelled
        if (current->children[letnum] == NULL)
        {
            return false;
        }   
            
        // else the word isn't finished yet
        else current = current->children[letnum];                   
    }
    
    if (current->is_word == true) return true;
       
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{                    
    // open dictionary specified in command line
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        
        printf("Could not open %s.\n", dictionary);
        return false;
    }
   
    char d;
    dTrie.root = createNode();
    trie_node* current; // pointer for where you're at
    current = dTrie.root;
    
    while ((d = fgetc(fp)) != EOF)
    {        
                                           
       // if \n and at least one letter has been indexed (to avoid empty new lines)
       if ((d == '\n') && current != dTrie.root)
       { 
            // reset the count through the nodes, create a leaf, set current back to root
           
            current->is_word = true;
            dTrie.wordcount++;
            current = dTrie.root;            
        }
            
       else if (isalpha(d) || (d == '\'')) // alphabet or apostrophe, add another node
       {            
            int letnum = lettermath(d);
            
            if (current->children[letnum] == NULL) // if you haven't allocated memory for this particular linked node before
            {
                current->children[letnum] = createNode();                           
            }
            
            current = current->children[letnum];
            
        }                                        
    }
    
    // check whether there was an error with the file
    if (ferror(fp))
    {
        fclose(fp);
        printf("Error reading %s.\n", dictionary);        
        return 1;
    }

    // close text
    fclose(fp);     
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // use count attribute of trie
    int count_success = dTrie.wordcount;
    return count_success;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // 
    deleteChildren(dTrie.root);
    
    return true;
    
}
   
    

