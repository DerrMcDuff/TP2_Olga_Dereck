// Fichier header pour d��finir les deux fonctions tu TP 2

// Noter que les fonctions d'appellent mymalloc et myfree
// au lieu de malloc et free.

#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#include <stddef.h>

struct bloc;

struct region;

region *startRegion;

void new_bloc;

void new_rgn;

void *mymalloc(size_t size);

void myfree(void *ptr);

#endif // MYMALLOC_H_
