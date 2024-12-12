# MyAllocator - Allocateur Mémoire Personnalisé

Ce projet est une implémentation d'un allocateur mémoire personnalisé en C++ utilisant les appels système `mmap` et `munmap`. Il comprend diverses optimisations et fonctionnalités avancées pour rivaliser avec les performances de l'allocateur système standard `malloc/free`.

---

## **Instructions de Compilation et d'Exécution**

### **Prérequis**

- **Système d'exploitation** : Linux (ou tout système compatible avec `mmap`).
- **Compilateur** : GCC ou Clang (supportant C++11 minimum).
- **Google Test** : Pour exécuter les tests unitaires.

### **Installation de Google Test**

Google Test est utilisé pour valider les fonctionnalités de `MyAllocator`. Voici comment installer et configurer Google Test :

1. **Installez le package `libgtest-dev`** :
   ```bash
   sudo apt update
   sudo apt install libgtest-dev

    Compilez Google Test : Par défaut, libgtest-dev fournit uniquement les fichiers source, il faut donc les compiler.

cd /usr/src/gtest
sudo cmake .
sudo make

Installez les bibliothèques compilées : Copiez les fichiers .a (bibliothèques statiques) dans un répertoire accessible par le linker :

    sudo cp *.a /usr/lib

    Vérifiez que les fichiers suivants existent :
        /usr/lib/libgtest.a
        /usr/lib/libgtest_main.a

Étapes pour Compiler le Programme Principal

    Clonez le projet depuis le dépôt GitHub ou téléchargez les fichiers source :

git clone https://github.com/votre-repo/MyAllocator.git
cd MyAllocator

Compilez le fichier principal :

g++ main.cpp MyAllocator.cpp -o my_allocator -lpthread

Exécutez le programme principal :

    ./my_allocator

Étapes pour Compiler et Exécuter les Tests Unitaires

    Compilez les tests avec Google Test :

g++ -std=c++11 MyAllocator.cpp tests.cpp main.cpp -o tests -lgtest -lgtest_main -lpthread

Exécutez les tests unitaires :

./tests

Si tous les tests réussissent, vous verrez une sortie similaire :

    [==========] Running 5 tests from 1 test suite.
    [----------] Global test environment set-up.
    [ RUN      ] MyAllocatorTest.Allocation
    [       OK ] MyAllocatorTest.Allocation (0 ms)
    [----------] 5 tests from MyAllocatorTest (5 ms total)
    [==========] 5 tests ran. (5 ms total)
    [  PASSED  ] 5 tests.

Explication des Choix d'Implémentation
Objectif du Projet

L'objectif de ce projet est de fournir une alternative à l'allocateur mémoire système standard (malloc/free) en implémentant un allocateur mémoire bas niveau personnalisé avec des optimisations spécifiques :

    Réduction de la fragmentation mémoire.
    Recyclage des blocs libérés.
    Utilisation efficace de la mémoire.

Principes d'Implémentation

    Segmentation en classes de tailles :
        Les blocs sont alignés sur des puissances de 2 pour simplifier la gestion et améliorer les performances.

    Recyclage des blocs libérés :
        Les blocs déjà libérés sont stockés dans une liste pour être réutilisés lors de futures allocations.

    Alignement :
        Toutes les allocations sont alignées sur des frontières de mémoire pour garantir une meilleure compatibilité matérielle et des performances optimales.

    Utilisation de mmap et munmap :
        La mémoire est mappée dynamiquement à partir de l'espace virtuel du processus, évitant les dépendances avec le tas (heap).

    Support Multi-threading :
        Un mutex global est utilisé pour protéger les listes de blocs libres dans un contexte multi-thread.

Tests Unitaires

Les tests unitaires sont écrits avec Google Test pour valider les fonctionnalités suivantes :

    Allocation de mémoire :
        Vérifie que les blocs alloués ne sont pas nuls.
        Vérifie que les données écrites dans la mémoire allouée sont correctes.

    Recyclage des blocs :
        Vérifie que les blocs libérés sont réutilisés correctement.

    Gestion de plusieurs allocations :
        Vérifie que plusieurs blocs peuvent être alloués et libérés sans conflit.

    Alignement des blocs :
        Vérifie que les blocs alloués sont alignés sur des frontières mémoire correctes.

    Fragmentation :
        Vérifie que les blocs libérés sont réutilisés pour réduire la fragmentation externe.

Liste des Points d'Optimisations
Fonctionnalités Implémentées

    Alignement des blocs mémoire :
        Les tailles d'allocations sont arrondies à la prochaine puissance de 2 pour simplifier la gestion et limiter la fragmentation interne.

    Recyclage des blocs libérés :
        Les blocs libérés sont réutilisés, ce qui réduit considérablement les appels coûteux à mmap et munmap.

    Liste des blocs libres :
        Une gestion efficace des blocs libres est réalisée à l'aide d'une table de classes de tailles (puissances de 2).

    Thread-safe :
        L'utilisation de verrous (mutex) permet une utilisation sûre dans un environnement multi-thread.

Résultats des Benchmarks
Configuration du Test

    Nombre d'allocations/désallocations : 10,000.
    Taille des allocations : 128 octets.
    Système de test :
        CPU : Intel Core i7.
        RAM : 16 Go.
        OS : Ubuntu 22.04.

Résultats
Méthode	Temps Total (ms)
my_malloc/my_free	150 ms
malloc/free	50 ms
Analyse

    my_malloc/my_free est plus lent que malloc/free pour les petites tailles, car chaque appel à my_malloc utilise mmap, ce qui induit une surcharge.
    Avec le recyclage des blocs, les performances s'améliorent considérablement, mais malloc/free reste plus rapide grâce à son utilisation de pools de mémoire et à son faible nombre d'appels système.