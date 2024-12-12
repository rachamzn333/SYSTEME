#ifndef MY_ALLOCATOR_H
#define MY_ALLOCATOR_H

#include <iostream>
#include <sys/mman.h>
#include <map>
#include <mutex>
#include <cmath>
#include <unistd.h>

class MyAllocator {
private:
    struct Block {
        size_t size;     
        bool free;       
        Block* next;      

        Block(size_t sz) : size(sz), free(true), next(nullptr) {}
    };

    std::map<size_t, Block*> freeLists; 
    size_t pageSize;                   
    std::mutex allocMutex;            

    size_t alignToPowerOfTwo(size_t size);  
    Block* findFreeBlock(size_t size);      
    void addFreeBlock(Block* block);         

public:
    MyAllocator();                          
    void* my_malloc(size_t size);            
    void my_free(void* ptr);                 
    void debugFreeLists();                   
};

#endif 



