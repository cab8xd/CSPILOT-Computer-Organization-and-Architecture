
#include<stdio.h>
#include<string.h>
#include"config.h"
#include"mlpt.h"
#include<stdlib.h>
#include<assert.h>


//handle up to 64 bits
size_t ptbr;

int check(){
    if((void*)ptbr==NULL)
    {
        return 1;
    }
    else{
        return 0;
    }
}

size_t entryAt(size_t va,size_t lvl){
    size_t y=va>>POBITS;
    size_t x=y>>(((POBITS-3)*(LEVELS-lvl)));
    size_t z=x&((1<<(POBITS-3))-1);
    return z;
}


//------------------------------------------------------
void page_allocate(size_t va){
    if(check()){
        void*ans;
        posix_memalign(&ans,1<<POBITS,1<<POBITS);
        memset(ans,0,1<<POBITS);
        ptbr=(size_t)ans;
    }
    
    size_t pte_ptr_address=ptbr;
    size_t*page_table_entry_address;
    size_t page_table_entry;
    for(size_t l=1;l<=LEVELS;l++){
        size_t VPN=entryAt(va,l)<<3;
        printf("VPN: %X",VPN);
        page_table_entry_address=(size_t*)((pte_ptr_address)+VPN);
        page_table_entry=*page_table_entry_address;
        if((page_table_entry&1)==0){
            void*ans;
            posix_memalign(&ans,1<<POBITS,1<<POBITS);
            memset(ans,0,1<<POBITS);
            page_table_entry=((size_t)ans)+1; //Setting the flag
            *page_table_entry_address=page_table_entry; //SEG FAULTING - FIXED
            pte_ptr_address=(size_t)ans;
        }
        else{
            size_t physical_page_number=page_table_entry; //Shift page table entry left 12 bits
            pte_ptr_address=page_table_entry-1;
        }
    }
}

size_t translate(size_t va){
    if(ptbr==0){
        return-1;
    }
    size_t pte_ptr_address=ptbr;  //Pointer
    size_t*page_table_entry_address;
    size_t page_table_entry;
    for(size_t l=1;l<=LEVELS;l++){
        printf("Level %X \n",l);
        size_t VPN=entryAt(va,l)<<3;
        page_table_entry_address=(size_t*)((pte_ptr_address)+VPN);
        page_table_entry=*page_table_entry_address;
        printf("B \n");
        
        if((page_table_entry&1)==0){
            printf("Empty\n");
            return -1;
        }
        else{
            printf("ELSE \n");
            if(l==LEVELS){
                printf("LAST LEVEL \n");
                size_t physical_page_number=page_table_entry; //Shift page table entry left 12 bits
                size_t ans=((1<<POBITS)-1)&va;
                size_t ans2=page_table_entry-1+ans;
                return ans2;
            }
        }
        printf("E \n");
        pte_ptr_address=page_table_entry-1;
        printf("to Next Level \n");
        
    }
    return-1;
}


//  //printf("Finished allocate\n \n.");










#include<stdio.h>
#include<assert.h>
#include"mlpt.h"
#include"config.h"

int main(){
    //printf("POBITS is %d \n", POBITS);
    // 0 pages have been allocated
    assert(ptbr==0);
    
    // //printf("Example is 0x123456789abcdef \n" );
    //size_t add = entryAt(0x123456789abcdef, 1);
    // //printf("Level 1: %zu \n", add);
    // return 0;
    
    printf("_FIRST_ALLOCATE_\n");
    page_allocate(0x123456789abcdef);
    // 5 pages have been allocated: 4 page tables and 1 data
    printf("Asserting...");
    assert(ptbr!=0);
    printf("_DONE_\n");
    
    
    printf("_SECOND_ALLOCATE_\n");
    page_allocate(0x123456789abcd00);
    page_allocate(0x123456789abcdef);
    // no new pages allocated (still 5)
    printf("_DONE_\n");
    
    
    //printf("_FIRST_TRANSLATE_\n");
    int*p1=(int*)translate(0x123456789abcd00);
    *p1=0xaabbccdd;
    short*p2=(short*)translate(0x123456789abcd02);
    printf("%04hx\n",*p2); // prints "aabb\n"
    printf("        Yikes \n");
    assert(translate(0x123456789ab0000)==0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x123456789ab0000);
    // 1 new page allocated (now 6; 4 page table, 2 data)
    
    assert(translate(0x123456789ab0000)!=0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x123456780000000);
    printf("\n \n DONE!!!!!!!!!");
    // 2 new pages allocated (now 8; 5 page table, 3 data)
    
    
}
