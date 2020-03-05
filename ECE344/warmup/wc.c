#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "wc.h"

//#define MAX_SIZE 100000

struct WordNode {
    char* word;
    int count;
    struct WordNode* next;
};

struct wc {
    int size; // Size of hash table
    struct WordNode** head;
};

// Using djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash_function(unsigned char* str)
{
    unsigned long hash = 5381;
    int c;
    
    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash;
}

// Insertion into hash table
void insert_wc(struct wc* wc, char* word, int index)
{
    // Look through the words at the current index to see if the word
    // already exists, if not add it to the end of the linked list
    struct WordNode* currentWord = wc->head[index];
    bool found = false;
    
    // If there are no words currently in the list, just put it in
    if(currentWord==NULL) {
        struct WordNode* newWord = (struct WordNode*)malloc(sizeof(struct WordNode));
        assert(newWord);
        newWord->word = word;
        newWord->count = 1; // Start count from 1
        newWord->next = NULL;
        
        wc->head[index] = newWord;
    } else {
        // Look through all currently inserted words, checks up to the last word
        while(currentWord->next!=NULL && !found) {
            if(strcmp(currentWord->word,word)==0) { // Word found, increment count
                found = true;
                currentWord->count++;
            } else currentWord = currentWord->next; // Move onto next word
        }

        // This checks the last word in the linked list
        if(!found && strcmp(currentWord->word,word)==0) {
            found = true;
            currentWord->count++;
        }
        
        // Add word onto end if not found, currentWord should be pointing to NULL
        if(!found) {
            struct WordNode* newWord = (struct WordNode*)malloc(sizeof(struct WordNode));
            assert(newWord);
            newWord->word = word;
            newWord->count = 1; // Start count from 1
            newWord->next = NULL;

            currentWord->next = newWord;
        }
    }
}

struct wc *
wc_init(char *word_array, long size)
{
    struct wc *wc;
    
    wc = (struct wc *)malloc(sizeof(struct wc));
    assert(wc);
    
    // Initialize hash table
    wc->size = size/10;
    wc->head = (struct WordNode**)malloc(sizeof(struct WordNode*)*wc->size);
    assert(wc->head);
    for(int i=0; i<wc->size; i++) { // Set all heads to point to NULL
        wc->head[i] = NULL;
    }
    
    // Parse word array, extract each word from array and insert into hash table
    int strStart = 0,strCount = 0;
    for(long i=0; i<size; i++) {
        // Word stops at space, also make sure that "" does not qualify as word
        if(isspace(word_array[i])) {
            // Ensures skipping over double spaces
            if(strCount==0) {
                strStart += 1;
            } else {
                char* string = (char*)malloc(sizeof(char)*(strCount+1));
                strncpy(string,word_array + strStart, strCount);

                unsigned long hash = hash_function((unsigned char*)string);

                insert_wc(wc,string,hash%(wc->size));
                
                // Reassign start of string, +1 to skip over the space
                strStart += (strCount+1);
                strCount = 0;
            }
        } else if(word_array[i]!='\n') strCount++;
    }
    
    return wc;
}

void
wc_output(struct wc *wc)
{
    // Go through hash table and print everything
    for(int i=0; i<wc->size; i++) {
        struct WordNode* currentWord = wc->head[i];
        
        while(currentWord != NULL) {
            printf("%s:%d\n",currentWord->word,currentWord->count);
            currentWord = currentWord->next;
        }
    }
}

void
wc_destroy(struct wc *wc)
{
    // Go through the hash table and free every non-null item
    for(int i=0; i<wc->size; i++) {
        struct WordNode* currentWord = wc->head[i];
        struct WordNode* nextWord;
        while(currentWord!=NULL) {
            nextWord = currentWord->next;
            free(currentWord->word);
            free(currentWord);
            currentWord = nextWord;
        }
        wc->head[i] = NULL;
    }
    free(wc->head);
    
    free(wc);
}
