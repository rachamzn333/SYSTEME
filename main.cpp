#include "MyAllocator.h"
#include <chrono>
#include <gtest/gtest.h>



void analyse_performance() {
    MyAllocator allocator;

    const size_t NUM_ALLOCATIONS = 10000;
    const size_t ALLOCATION_SIZE = 128;


    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NUM_ALLOCATIONS; ++i) {
        void* ptr = allocator.my_malloc(ALLOCATION_SIZE);
        allocator.my_free(ptr);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Temps pris par my_malloc/my_free : " << duration << " ms" << std::endl;


    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NUM_ALLOCATIONS; ++i) {
        void* ptr = malloc(ALLOCATION_SIZE);
        free(ptr);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Temps pris par malloc/free : " << duration << " ms" << std::endl;
}






int main(int argc, char** argv) {
    
    ::testing::InitGoogleTest(&argc, argv);

    MyAllocator allocator;

    allocator.debugFreeLists();

 
    analyse_performance();



    return RUN_ALL_TESTS();


}
