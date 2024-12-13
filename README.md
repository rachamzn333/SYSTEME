# MyAllocator - Allocateur Mémoire Personnalisé

Ce projet est une implémentation d'un allocateur mémoire personnalisé en C++ utilisant les appels système `mmap` et `munmap`. Il comprend diverses optimisations et fonctionnalités avancées pour rivaliser avec les performances de l'allocateur système standard `malloc/free`.

---

## **Instructions de Compilation et d'Exécution**

### **Prérequis**

- **Système d'exploitation** : Linux (ou tout système compatible avec `mmap`).
- **Compilateur** : GCC ou Clang (supportant C++11 minimum).
- **Google Test** : Pour exécuter les tests unitaires.

---

### **Installation de Google Test**

#### 1. Installation

```bash
1. sudo apt update
2. sudo apt install libgtest-dev
```

#### 2. Compilation des sources

```bash
1. cd /usr/src/gtest
2. sudo cmake .
3. sudo make
```

#### 3. Installation des bibliothèques

```bash
1. sudo cp *.a /usr/lib
```

Vérifiez que les fichiers suivants existent :

- `/usr/lib/libgtest.a`
- `/usr/lib/libgtest_main.a`

---

### **Compilation et Exécution du Programme Principal**

#### 1. Clonez le projet

```bash
1. git clone git@github.com:rachamzn333/SYSTEME.git
2. cd SYSTEME
```

### **Compilation et Exécution des Tests Unitaires**

#### 1. Compilation

```bash
1. g++ MyAllocator.cpp tests.cpp main.cpp -o tests -lgtest -lgtest_main -lpthread
```

#### 2. Exécution

```bash
1. ./tests
```

Si tous les tests réussissent, la sortie ressemblera à ceci :

```
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[ RUN      ] MyAllocatorTest.Allocation
[       OK ] MyAllocatorTest.Allocation (0 ms)
[----------] 5 tests from MyAllocatorTest (5 ms total)
[==========] 5 tests ran. (5 ms total)
[  PASSED  ] 5 tests.
```

---

## **Explication des Choix d'Implémentation**

### **Objectif du Projet**

L'objectif de ce projet est de fournir une alternative à l'allocateur mémoire système standard (`malloc/free`) en implémentant un allocateur mémoire bas niveau personnalisé avec des optimisations spécifiques comme :

- Réduction de la fragmentation mémoire.
- Recyclage des blocs libérés.
- Utilisation efficace de la mémoire.

### **Principes d'Implémentation**

- **Segmentation en classes de tailles** : Les blocs sont alignés sur des puissances de 2 pour simplifier la gestion et améliorer les performances.
- **Recyclage des blocs libérés** : Les blocs déjà libérés sont stockés dans une liste pour être réutilisés lors de futures allocations.
- **Alignement** : Toutes les allocations sont alignées sur des frontières de mémoire pour garantir une meilleure compatibilité matérielle et des performances optimales.
- **Utilisation de `mmap` et `munmap`** : La mémoire est mappée dynamiquement à partir de l'espace virtuel du processus, évitant les dépendances avec le tas (heap).
- **Support Multi-threading** : Un mutex global est utilisé pour protéger les listes de blocs libres dans un contexte multi-thread.

---

## **Tests Unitaires**

Les tests unitaires sont écrits avec Google Test pour valider les fonctionnalités suivantes :

- **Allocation de mémoire** :
  - Vérification que les blocs alloués ne sont pas nuls.
  - Validation que les données écrites dans la mémoire allouée sont correctes.
- **Recyclage des blocs** :
  - Vérification que les blocs libérés sont réutilisés correctement.
- **Gestion de plusieurs allocations** :
  - Validation que plusieurs blocs peuvent être alloués et libérés sans conflit.
- **Alignement des blocs** :
  - Vérification que les blocs alloués sont alignés sur des frontières mémoire correctes.
- **Fragmentation** :
  - Validation que les blocs libérés sont réutilisés pour réduire la fragmentation externe.

---

## **Liste des Points d'Optimisations**

### **Fonctionnalités Implémentées**

1. **Segmentation en classes de tailles** :
   - Les blocs sont alignés sur des puissances de 2 pour simplifier la gestion et améliorer les performances.

2. **Recyclage des blocs libérés** : 
   - Les blocs déjà libérés sont stockés dans des caches locaux par thread pour minimiser les verrous globaux.

3. **Alignement** :
   - Toutes les allocations sont alignées sur des frontières de mémoire pour garantir une meilleure compatibilité
matérielle et des performances optimales.

   
4. **Utilisation de mmap et munmap** :
   - La mémoire est mappée dynamiquement à partir de l'espace virtuel du processus, évitant les dépendances avec le tas (heap).

5. **Support Multi-threading avec Arènes** :
   - Plusieurs arènes sont utilisées pour limiter la contention entre threads, chaque arène ayant son propre mutex.

6. **Détection des fuites mémoire** :
   - Un mécanisme de suivi des allocations détecte les blocs alloués mais non libérés à la fin de l'exécution.

7. **Optimisation des métadonnées** :
   - Les métadonnées pour chaque bloc sont compactes, contenant uniquement la taille, l'état du bloc, et un pointeur pour la gestion des listes.

---

## **Résultats des Benchmarks**

### **Configuration du Test**

- **Nombre d'allocations/désallocations** : 10,000.
- **Taille des allocations** : 128 octets.


### **Résultats**

| Méthode             | Temps Total (ms) |
| ------------------- | ---------------- |
| `my_malloc/my_free` | 70 ms           |
| `malloc/free`       | 0 ms            |

### **Analyse**

- `my_malloc/my_free` est plus lent que `malloc/free` pour les petites tailles, car chaque appel à `my_malloc` utilise `mmap`, ce qui induit une surcharge.
- Avec le recyclage des blocs, les performances s'améliorent considérablement, mais `malloc/free` reste plus rapide grâce à son utilisation de pools de mémoire et à son faible nombre d'appels systèmes.

