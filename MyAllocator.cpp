#include "MyAllocator.h"

MyAllocator::MyAllocator() {
    pageSize = getpagesize(); // Obtenir la taille de page du système
}

size_t MyAllocator::alignToPowerOfTwo(size_t size) {
    if (size <= 8) return 8; // Taille minimale alignée
    return 1UL << static_cast<size_t>(std::ceil(std::log2(size)));
}

MyAllocator::Block* MyAllocator::findFreeBlock(size_t size) {
    auto it = freeLists.lower_bound(size);
    if (it != freeLists.end() && it->second) {
        // Trouver un bloc dans la bonne classe de taille
        Block* block = it->second;
        freeLists[size] = block->next; // Retirer ce bloc de la liste des libres
        block->free = false;          // Marquer comme occupé
        block->next = nullptr;
        return block;
    }
    return nullptr; // Aucun bloc libre trouvé
}

void MyAllocator::addFreeBlock(Block* block) {
    block->free = true; // Marquer le bloc comme libre
    size_t alignedSize = alignToPowerOfTwo(block->size);
    block->next = freeLists[alignedSize]; // Ajouter en tête de la liste
    freeLists[alignedSize] = block;
}

void* MyAllocator::my_malloc(size_t size) {
    std::lock_guard<std::mutex> lock(allocMutex); // Protéger les allocations en multi-thread

    // Ajouter la taille des métadonnées au bloc
    size_t totalSize = alignToPowerOfTwo(size + sizeof(Block));

    // Rechercher un bloc libre
    Block* block = findFreeBlock(totalSize);
    if (block) {
        return static_cast<void*>(block + 1); // Retourner l'adresse utilisateur
    }

    // Si aucun bloc libre, allouer un nouveau bloc avec mmap
    void* mem = mmap(nullptr, totalSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return nullptr;
    }

    block = new (mem) Block(totalSize - sizeof(Block));
    block->free = false; 
    return static_cast<void*>(block + 1); 
}

void MyAllocator::my_free(void* ptr) {
    if (!ptr) return;

    std::lock_guard<std::mutex> lock(allocMutex); 

    Block* block = static_cast<Block*>(ptr) - 1; 
    addFreeBlock(block);                         
}

void MyAllocator::debugFreeLists() {

    std::cout << "État des blocs libres :" << std::endl;
    for (const auto& [size, head] : freeLists) {
        std::cout << "  Taille " << size << " : ";
        Block* current = head;
        while (current) {
            std::cout << "[" << current->size << (current->free ? " libre" : " occupé") << "] -> ";
            current = current->next;
        }
        std::cout << "nullptr" << std::endl;
    }

}
