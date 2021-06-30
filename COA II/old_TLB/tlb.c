//
//  tlb.c
//  
//
//  Created by Christine Baca on 3/19/19.
//

//Need to make set data structure

#include <stdio.h>
#include "config.h" /* see PA02 for guidance on this file */
#include "mlpt.h"  /* see PA02 this file */
#include "tlb.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


//TLB array

/** STUB **/
/** stub for the purpose of testing tlb_* functions */
size_t translate(size_t va) {  return va < 0x1234000 ? va + 0x20000 : -1; }


/** LINE STRUCT **/
//Question: should this be stored within an overarching table (another set)
typedef struct {
    int lru; //Least Recently Used
    size_t valid_bit; //Valid bit
    size_t tag; // Stores va high order bit
} cache_line;


/** GLOBALS **/
int levels = 4;
int sets = 16;

typedef struct{
    cache_line lines[4];
} cache_set;

cache_set tlb_set[16];



/** HELPER FUNCTIONS **/



/**returns set index, the middle 4 bits of a virtual address**/
size_t getSetIndex(size_t va)
{
    size_t ans;
    //shifting
    size_t setIndex = va >> POBITS;
    setIndex = (setIndex << 4) - 1;
    ans = setIndex;
    return ans;
    
}

/**returns tag of the va (high order bits)**/
size_t getTag(size_t va)
{
    size_t ans;
    size_t tag = va >> (POBITS + 4);
    ans = tag;
    return ans;
    
}
/**Prints cache**/
void printCache(){
    int lvl;
            printf("\n---------");
    for(lvl = 0;lvl < levels; lvl++){

        printf("\n lvl: %d  ", lvl);
        printf("[valid: %d | tag: %xu | lru: %d ] \n",tlb[lvl].valid_bit, tlb[lvl].tag, tlb[lvl].lru);
        printf("size of tag: %d ", sizeof(tlb[lvl].tag));

    }
            printf("---------\n");
    
}

/** returns lvl of least recently used bit **/
int getLRU(){
    int lvl;
    for(lvl = 0;lvl < levels; lvl++){
        //Initial change
        if(tlb[lvl].lru == levels)
        {
            return lvl;
        }
    }
    return -1;
}
/** returns valid bit **/
size_t valid(cache_line line){ return line.valid_bit;}

/** sets new LRU. Note: lru = 1 for MOST RECENTLY used **/
void resetLRU(int set){
    int old_lru = tlb[set].lru;
    int lvl;
    for(lvl = 0;lvl < levels; lvl++){
        //Initial change
        if(lvl == set)
        {
            tlb[lvl].lru = 1;
        }
        else if(tlb[lvl].lru <= old_lru)
        {
            tlb[lvl].lru += 1;
        }
    }
}



/** finds a new physical address **/
size_t find_new_address(size_t va, int lvl)
{
    size_t ans;
    size_t new_pa = translate(va);
    size_t setIndex = getSetIndex(va);
    // printf("New Pa: %xu...", new_pa);
    if(new_pa == -1)
    {
        // printf("...ERROR A. Done. \n");
        return -1;
    }
    
    //Finding tag
    tlb_set[setIndex][lvl].valid_bit = 1;
    tlb_set[setIndex][lvl].tag = getTag(va);
    ans = new_pa;
    return ans;
}

/** checks for mapped address **/
int is_mapped(va, cache_line tlb){
    size_t va_tag = getSetIndex(va); //The shift
    size_t ans;
    int lvl;
    for(lvl = 0;lvl < levels; lvl++){
        if(valid(tlb[lvl]))
        {
            size_t tag = tlb[lvl].tag; //Should be shifted already
            // printf("\ntag is %xu", tag);
            // printf("\nva_tag is %xu\n", va_tag);
            if(tag == va_tag)
            {
                ans = lvl;
             // printf("\nMapped answer is: %xu\n", ans);
                return ans;
            }
        }
    }
    ans = -1;
         // printf("\nMapped answer is: %xu\n", ans);
    return ans;
}



/** METHODS **/
/** invalidate all cache lines in the TLB */
void tlb_clear()
{
   // // printf("Clearing...");
    int lvl;
    int i;
    for(i = 0; i < 16; i++)
    {
        for(lvl = 0;lvl < levels; lvl++){
            tlb_set[i][lvl].valid_bit = 0; //Sets all valid bits to zero
        }

    }
    // printf("DONE\n");
}

/**
 * return 0 if this virtual address does not have a valid
 * mapping in the TLB. Otherwise, return its LRU status: 1
 * if it is the most-recently used, 2 if the next-to-most,
 * etc.
 */

//CHECK
int tlb_peek(size_t va){    //ERROR in tag is_mapped
    int ans;
   // // printf("Peeking...");
    int lvl;
    size_t setIndex = getSetIndex(va);
    if((lvl = is_mapped(va, tlb_set[setIndex])) != -1)
    {
        ans = tlb_set[setIndex][lvl].lru;
        resetLRU(lvl);
        return ans;
    }
//    // printf("...ans is %xu...Done\n ", ans);
    ans = 0;
    return ans;
   // return tlb[lvl].lru;
}

/**
 * If this virtual address is in the TLB, return its
 * corresponding physical address.
 
 If not, use
 * `translate(va)` to find that address, store the result
 * in the TLB, and return it. In either case, make its
 * cache line the least-recently used in its set.
 *
 * As an exception, if translate(va) returns -1, do not
 * update the TLB: just return -1.
 */
size_t tlb_translate(size_t va){
    // printf("Translating...");
    size_t ans;
    size_t shifted_tag; //Extra variable for trouble shooting.
    size_t setIndex = getSetIndex(va);
    int lvl = is_mapped(va, tlb_set[setIndex]);
    
    //Mapping is in page table
    if( is_mapped(va,tlb_set[setIndex]) != -1)
    {
        // printf("A");
        resetLRU(lvl);
        //shifted_tag = (tlb_set[setIndex][lvl].tag << bit_offset);
        ans = tlb_set[setIndex]
        // printf("..combined, ans is %xu...Done\n ", ans);
        return ans;
    }
    
    //Mapping is NOT in page table.
    lvl = 0;
    for(lvl = 0; lvl < levels; lvl++)
    {
       if(valid(tlb[lvl]) == 0)
       {
           // printf("B...");
           ans = find_new_address(va, lvl);
           resetLRU(lvl);
        // printf("translate_ans is %xu...Done\n ", ans);
           return ans ;
       }
    }
    
    //Page table has no space left.
    // printf("C...");
    lvl = getLRU();
    ans =  find_new_address(va, lvl);
    resetLRU(lvl);
    // printf("translate_ans is %xu...Done\n ", ans);
    return ans;
}



/*
 Example: If bit_offset (POBITS) is 12 and tlb_translate is invoke with addresses 0x12345, 0x12468, and 0x13579 in that order,
 
 tlb_translate(0x12345) should invoke translate(0x12000),
 tlb_translate(0x12468) should not invoke translate at all (it’s a cache hit), and
 tlb_translate(0x13579) should not invoke translate(0x13000)
 */


/** MAIN FUNCTION **/
int main(){
    tlb_clear();
    assert(tlb_peek(0) == 0); //Done.
    assert(tlb_translate(0) == 0x20000); //DONE, 1
    assert(tlb_peek(0) == 1);

    assert(tlb_translate(0x200) == 0x20200); //FAILED

    assert(tlb_peek(0) == 1); //DONE
    assert(tlb_peek(0x200) == 1); //DONE
    assert(tlb_translate(0x1200) == 0x21200); //3
    assert(tlb_translate(0x5200) == 0x25200); // 4
    assert(tlb_translate(0x8200) == 0x28200);
    assert(tlb_translate(0x2200) == 0x22200);   /// ALL IN THE TWOS PLACE, ALL SHOULD HIT TRANSLATE?
    printCache();
    //TRANSLATE AND PEEK LOOKING FOR DIFFERENT THINGS??
    assert(tlb_peek(0x1000) == 1); //FIXED: shift bit_offset right to isolate high order bits - lru?
    assert(tlb_peek(0x5000) == 1); //ERROR, not there TAGS ARE WRONG RE LOOK AT NOTES AND GO TO OFFICE HOURS
    assert(tlb_peek(0x8000) == 1);
    assert(tlb_peek(0x2000) == 1);
    assert(tlb_peek(0x0000) == 1);
    assert(tlb_translate(0x101200) == 0x121200);
    assert(tlb_translate(0x801200) == 0x821200);
    assert(tlb_translate(0x301200) == 0x321200);
    assert(tlb_translate(0x501200) == 0x521200);
    printCache();
   
    assert(tlb_translate(0xA01200) == 0xA21200);
    assert(tlb_translate(0xA0001200) == -1);
  
    printCache();
    assert(tlb_peek(0x001200) == 0); //FIXED: setLRU fix

    assert(tlb_peek(0x101200) == 0);
                  printf("\n \n");
       printCache();
    assert(tlb_peek(0x301200) == 3);
             printf("TEST DONE\n.");
    assert(tlb_peek(0x501200) == 2);
    assert(tlb_peek(0x801200) == 4);
    assert(tlb_peek(0xA01200) == 1);
    assert(tlb_translate(0x301800) == 0x321800);
    assert(tlb_peek(0x001000) == 0);
    assert(tlb_peek(0x101000) == 0);
    assert(tlb_peek(0x301000) == 1);
    assert(tlb_peek(0x501000) == 3);
    assert(tlb_peek(0x801000) == 4);
    assert(tlb_peek(0xA01000) == 2);
    return 0;
}
 
 
