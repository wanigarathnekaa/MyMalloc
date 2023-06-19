/*
    SCS1214 - Operating Systems I
    2100071 - K.A.A.Wanigarathne
*/

#include<stdio.h>
#include<stdbool.h>
#define MemorySize 25000
#define start 0


static int pos = 0; //Work as a position variable
void *ptr;
static int init = 0;
static int free_memory = 0;

//Array of Free Spaces
char memory[MemorySize];

//Structure of the meta data Block
struct node{
    struct node *next;
    int size;
    int position;
};
static struct node *head = (struct node*)&memory[0];
static struct node *temp_free;


void *MyMalloc(size_t size){

    static struct node* ptr_free = (struct node*)&memory[0]; //ptr_free Allocates at the 0th position of the char array 
    static struct node *ptr_allo; // ptr_allo is the pointer for the Allocation Block

    if(size == 0 || head == NULL){
        printf("Cannot Allocate Memory.\n"); //This is to avoid getting junk allocations.
    }
    else if(size > 0){
        if(init == 0){ //Allocating Memory for the first time

            //Free Block
            head = ptr_free;
            ptr_free->next = ptr_free;
            free_memory = MemorySize - sizeof(struct node);
            ptr_free->size = free_memory;
            init ++;
        }
        
        if(init == 1 && free_memory > size){
            //Allocated Block
            ptr_allo = ptr_free;
            ptr_allo->next = NULL;
            ptr_allo->size = size;

            //ptr_free = ptr_free + size + sizeof(struct node); // Need to traverse along the char array
            //Free Block
            pos += size + sizeof(struct node);

            ptr_free = (struct node *)&memory[pos];
            free_memory -= (sizeof(struct node) + size);
            ptr_free->position = pos;
            ptr_free->size = free_memory;
            ptr_free->next = ptr_free;
            head = ptr_free;
            init ++;

            return (void *)(ptr_allo+1);
        }
        else{
            //Start searching the the first fit free block.
            //It starts to search from first ever free block!!
            ptr_free = temp_free;
            struct node *te, *check = temp_free;
            struct node * temp_allo = ptr_allo;
            if(head != NULL)
                temp_free = head;

            int i=init;

            while(i){
                if(temp_free->size == size){

                    temp_allo = temp_free;
                    temp_allo->size = size;
                    temp_allo->position = temp_free->position;

                    if(head->next == temp_free){
                        head->next = temp_free->next;
                    }
                    else{
                        check = head;
                        while(check->next != temp_free){
                            check = check->next;
                        }
                        temp_free = temp_free->next;
                        check->next = temp_free;
                    }

                    return (void *)(temp_allo+1);
                }
                else if(temp_free->size > size)
                {
                    int temp_pos;
                    if(temp_free->size > size + sizeof(struct node))
                    {
                        int temp_pos1, flag = 0, tempsize;

                        tempsize = temp_free->size;
                        temp_pos = temp_free->position;
                        temp_pos1 = temp_pos;
                        te = temp_free;
                        if(te == temp_free->next){
                            flag = 1;
                        }

                        temp_pos += size + sizeof(struct node);
                        temp_free = (struct node*)&memory[temp_pos];
                        if(flag == 1){
                            temp_free->next = temp_free;
                        }
                        else{
                            temp_free->next = te->next;
                        }

                        //Maintaining the first fit property and setting the next of next
                        if(check > head && te != head){
                            check = head;
                            while(check->next != te){
                                check = check->next;
                            }
                            check->next = temp_free;
                        }
                        

                        temp_free->size = tempsize - (sizeof(struct node) + size);
                        temp_free->position = temp_pos;

                        if(head->next == head || te == head){
                            head = temp_free;
                        }

                        temp_allo = te;
                        temp_allo->next = temp_free;
                        temp_allo->size = size;
                        temp_allo->position = temp_pos1;
                            
                        pos += size + sizeof(struct node);
                        init++;
                            
                        return (void *)(temp_allo+1);
                    }
                    else{
                        int flg = 0;
                        temp_allo = temp_free;
                        check = head;
                        if(head == temp_free){
                            flg = 1;
                        }
                        while(head != temp_free && check->next != temp_free){
                            check = check->next;
                        }
                        temp_free = temp_free->next;
                        check->next = temp_free;
                        
                        if((head->position + size + 32) > 25000)    
                            head = NULL;
                        
                        //since temp_free and head are in same position, if temp_free changes..Head also should change
                        if(flg == 1 && head != NULL){
                            head = ptr_free;
                        }
 
                        return(temp_allo+1);
                    }
                }
                else{
                    if(temp_free == temp_free->next){
                        printf("Fail to Allocate the Memory\n");
                        break;
                    }
                    temp_free = temp_free->next;
                }
                i--;
            }
        }

    }
}


//My Free Function is for freeing Allocated memory(Which means Adding the Allocted memory blocks in to the array of free memory blocks.)
void MyFree(void *ptr)
{
    struct node *block;
    struct node *ftemp;
    struct node *ftemp2;


    block = (struct node *)ptr;
    block -= 1; 

    if(head > block && ((void*)(block+1)+block->size) == head){
        //blockClear(block);
        if(head->next == head){
            block->next = block; 
        }
        else{
            block->next = head->next;   
        }
        block->size += head->size + sizeof(struct node);
        head = block;
    }
    else if(head > block){
        block->next = head;
        head = block;
        //blockClear(block);
    }
    else if(head == NULL){
        head = block;
        head->next = head;
    }
    else{
        if(head->next == head){
            head->next = block;
            block->next = block;
            //blockClear(block);
        }
        else{
            ftemp = head; 
            while (ftemp->next != ftemp){
                ftemp = ftemp->next;
                //Something Like Merging
                if(((void*)(ftemp+1)+ftemp->size) == block){
                    ftemp->size = block->size + sizeof(struct node);
                    //blockClear(block);
                    break;
                }
                else if(ftemp > block && ((void*)(block+1)+block->size) == ftemp){
                    block->size += ftemp->size + sizeof(struct node); 
                    block->next = ftemp->next;
                    ftemp = block;
                    //blockClear(block);
                    break;
                } 
                else if(ftemp > block && ((void*)(head+1)+head->size) == block){
                    head->size += block->size + sizeof(struct node);
                    break;
                    //blockClear(block); 
                } 
                else if(ftemp > block){
                    //blockClear(block);
                    ftemp2 = head;
                    while (ftemp2->next != ftemp){
                        ftemp = ftemp->next;
                    }
                    block->next = ftemp;
                    ftemp2->next = block;
                    
                    break;
                }   
            }
        }
    }
}
