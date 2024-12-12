#include "MyAllocator.h"
#include <gtest/gtest.h>
#include <cstring> // Pour strcpy et strcmp

// Test simple d'allocation
TEST(MyAllocatorTest, Allocation) {
    MyAllocator allocator;

    // Allouer un bloc de 128 octets
    void* ptr = allocator.my_malloc(128);
    ASSERT_NE(ptr, nullptr) << "L'allocation a échoué : pointeur nul retourné.";

    // Écrire dans la mémoire
    strcpy(static_cast<char*>(ptr), "Hello Google Test");
    ASSERT_STREQ(static_cast<char*>(ptr), "Hello Google Test") << "Les données écrites ne correspondent pas.";

    // Libérer la mémoire
    allocator.my_free(ptr);
}

// Test du recyclage des blocs libérés
TEST(MyAllocatorTest, Recycling) {
    MyAllocator allocator;

    // Allouer et libérer un bloc
    void* ptr1 = allocator.my_malloc(64);
    allocator.my_free(ptr1);

    // Réallouer un bloc de même taille
    void* ptr2 = allocator.my_malloc(64);

    // Vérifier que le même bloc est réutilisé
    ASSERT_EQ(ptr1, ptr2) << "Le bloc libéré n'a pas été réutilisé.";

    allocator.my_free(ptr2);
}

// Test de plusieurs allocations
TEST(MyAllocatorTest, MultipleAllocations) {
    MyAllocator allocator;

    // Allouer plusieurs blocs de tailles différentes
    void* ptr1 = allocator.my_malloc(32);
    void* ptr2 = allocator.my_malloc(64);
    void* ptr3 = allocator.my_malloc(128);

    // Vérifier que les allocations ne retournent pas de pointeurs nuls
    ASSERT_NE(ptr1, nullptr);
    ASSERT_NE(ptr2, nullptr);
    ASSERT_NE(ptr3, nullptr);

    // Libérer les blocs
    allocator.my_free(ptr1);
    allocator.my_free(ptr2);
    allocator.my_free(ptr3);
}

// Test de l'alignement des blocs
TEST(MyAllocatorTest, Alignment) {
    MyAllocator allocator;

    // Allouer un bloc
    void* ptr = allocator.my_malloc(50);

    // Vérifier que le bloc est aligné sur 8 octets
    ASSERT_EQ(reinterpret_cast<uintptr_t>(ptr) % 8, 0) << "Le bloc n'est pas correctement aligné.";

    allocator.my_free(ptr);
}

// Test de gestion de fragmentation
TEST(MyAllocatorTest, Fragmentation) {
    MyAllocator allocator;

    // Allouer et libérer plusieurs blocs
    void* ptr1 = allocator.my_malloc(128);
    void* ptr2 = allocator.my_malloc(64);
    allocator.my_free(ptr1);

    // Réallouer un bloc de même taille que le premier
    void* ptr3 = allocator.my_malloc(128);

   
    ASSERT_EQ(ptr1, ptr3) << "Le bloc libéré n'a pas été réutilisé.";

    allocator.my_free(ptr2);
    allocator.my_free(ptr3);
}

