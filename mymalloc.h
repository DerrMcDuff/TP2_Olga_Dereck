// Fichier header pour d��finir les deux fonctions tu TP 2

// Noter que les fonctions d'appellent mymalloc et myfree
// au lieu de malloc et free.

#ifndef MYMALLOC_H_
#define MYMALLOC_H_

#include <stddef.h>

typedef struct bloc;

typedef struct region;

int roundUp(size_t size);

bloc *lastBloc = NULL;
bloc *lastRgn = NULL;

region *startRegion;

void new_bloc(void* addr);

void new_rgn(size_t size, void* addr)

void *mymalloc(size_t size);

region *createLastReg(region *reg, int size);

void myfree(void *ptr);

void freeRegion(region *dyingRgn, int direction);

void mergeRegions(region *rgn1, region *rgn2);


#endif // MYMALLOC_H_
