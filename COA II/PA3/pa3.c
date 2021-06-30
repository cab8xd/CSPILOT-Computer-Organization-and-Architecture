/*
 Christne Baca cab8xd
 Worked with ssb7xx
 */
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "mlpt.h"
#include <stdlib.h>
#include <assert.h>

size_t ptbr;
/**
 Method: check()
 Deduces whether the ptbr is empty. If empty, returns 1, otherwise returns 0
 **/
int check()
{
    if((void*)ptbr == NULL)
    {
        return 1;
    }
    else{
        return 0;
     }
}
/**
 Method entryAt(size_t <virtual_addresss>, size_t <current_level>)
 Returns the virtual page addres or vpn of the current level from the virtual address.
 **/
size_t entryAt(size_t va, size_t lvl)
{
    size_t vpn = ((va >> POBITS) >> ((POBITS - 3) * (LEVELS - lvl))) & ((1 << (POBITS - 3)) - 1);
    return vpn;
}

/** 
Method page_allocate(size_t <virtual_address>)
Allocates new memory for LEVELS number of pages in the page table. 
Allocates a new page table if the page table register has not been allocated already.
**/
void page_allocate(size_t va)
{
   //Initializes memory for page table base register if allocating for the first time
    if(check()){
        void *ans;
        posix_memalign(&ans, 1 << POBITS, 1 << POBITS);
        memset(ans,0,1 << POBITS);
        ptbr =(size_t) ans;
    }
	
    size_t pte_ptr_address = ptbr;
    size_t* page_table_entry_address;
    size_t page_table_entry;
    //Loops through and initializes memory for pages of the page table
    for(size_t l = 1; l <= LEVELS; l = l + 1)
    {
        size_t vpn = entryAt(va, l)  <<  3;
        page_table_entry_address = (size_t*)((pte_ptr_address) + vpn);
        page_table_entry = *page_table_entry_address;
	//If the page does not exist, allocates new memory for a the table and moves to the next page
        if((page_table_entry & 1) == 0)
        {
            void *ans;
            posix_memalign(&ans, 1 << POBITS, 1 << POBITS);
            memset(ans,0,1 << POBITS);
            page_table_entry = ((size_t)ans) + 1;
            *page_table_entry_address= page_table_entry;
            pte_ptr_address = (size_t)ans;
        }
	//If the current page's memory already exists, moves to the next page
        else
        {
            size_t physical_page_number = page_table_entry;
            pte_ptr_address = page_table_entry - 1;
        }
    }
}
/** 
Method translate(size_t <virtual address>)
Returns the physical address indicated by the inputted virtual address.
If the address leads to an unallocated page or invalid address, returns -1
**/
size_t translate(size_t va)
{
    //If the page table does not exists, program ends and returns -1
    if(ptbr == 0)
        return -1;
    size_t pte_ptr_address = ptbr;
    size_t* page_table_entry_address;
    size_t page_table_entry;
    //Loops through pages of the page table
    for(size_t l = 1; l <= LEVELS; l = l + 1)
    {
        size_t vpn = entryAt(va, l)  <<  3;
        page_table_entry_address = (size_t*)((pte_ptr_address) + vpn);
        page_table_entry = *page_table_entry_address;
	//If the page does not exist or memory has not been allocated for the page, returns -1
        if((page_table_entry & 1) == 0){
            return -1;       
        }
        else
        {
	    //If the page table is at its final level, returns the physical address
            if(l == LEVELS)
            {
                size_t physical_page_number = (page_table_entry - 1) + ((( 1 << POBITS ) - 1) & va);
                return physical_page_number;
            }
        }
	   //Moves on to the nexts page.
            pte_ptr_address = page_table_entry - 1;
        
    }
    //Ends method if program finishes for loop
    return -1;
}
    
    

 



    
    
    
    
   
    
    
      

