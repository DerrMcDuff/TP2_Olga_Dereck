// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit ??tre dans ce fichier

#include <stdio.h>
#include <sys/mman.h>
#include <math.h>
#include "mymalloc.h"


typedef struct {

  int occ; //si le bloc est occupee occ vaut 1, sinon 0
  int nxtLinked; // si le bloc est lie a son precedant prevLinked vaut 1, sinon 0
  void* start;
  void* end;
  size_t size;
  struct bloc* nxt;
  
} bloc;

typedef struct {

  int occ; //si le bloc est occupee occ vaut 1, sinon 0
  void* start;
  void* end;
  size_t size;
  struct region* prev;
  struct region* next;
  
} region;

bloc lastBloc = NULL;
bloc lastRgn = NULL;

//les pages sont compose de 4096 alloue par mmap
void new_bloc(void* addr) {

  bloc newBloc;
  
  newBloc.occ = 1;
  newBloc.nxtLinked = 0;
  newBloc.start = mmap(addr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  newBloc.end = newBloc.start + 4096;
  newBloc.size = 4096 - (sizeof(size_t) + sizeof(bloc));
  newBloc.prv = lastBloc;
  newBloc.nxt = NULL;
  if(lastBloc != NULL){
    lastBloc.nxt = newBloc;
  }

  lastBloc = newBloc;

}



//chaque page va contenir plusieurs blocs
void new_rgn(size_t size, void* addr) {

  bloc newRgn ;

  newRgn.occ = 1;
  newRgn.start = addr;
  newRgn.end = addr + size;
  newRgn.size = size - (sizeof(size_t) + sizeof(bloc));
  newRgn.prv = lastRgn;
  newRgn.nxt = NULL;
  if(lastRgn != NULL){
    lastRgn.nxt = newRgn;
  }

  lastRgn = newRgn;
}



void *mymalloc(size_t size){


  if(size < 4096){

    return NULL;

  }
  
  if (size > 4096) {

    int nbB = ceil(size/4096);

    if(lastBloc == NULL){
      new_bloc(NULL); // creation de la premiere page
    } else new_bloc(lastBloc.end);

    // creation de la region avec l'addresse du debut de la premiere page, et la taille donnee
    new_rgn(size, lastBloc.start);

    for(int i = 1; i<nbB; i++){
      new_bloc(lastBloc.end);
      lastBloc.nxtLinked = 1;
    }

    lastRgn.end = lastBloc.end;

    return lastBloc.start;

  }


region *startRegion;

//	void* addr = mmap(0, size + sizeof(size_t), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
//	*(size_t*)addr = size;
//	return addr + sizeof(size_t);

}

bloc *firstBloc;

void myfree(void *ptr){
  
  region *focusedRegion = firstBloc.start;
  int found = 0;
  
  while ((found == 0) && (focusedRegion.occ == 1) && (focusedRegion != NULL)) {
    if &focusedRegion == &ptr {
      found == 1;
      break;
    }
    focusedRegion = focusedRegion.next;
  }
  
  if (found == 1) {
    killRegion(&focusedRegion, 0);
  } else {
    printf("this thing was ")
  }
  
}

int killRegion(region *dyingRgn, int direction) {
  
  region *prevRgn = dyingRgn.prev;
  if prevRgn != NULL && (prevRgn.occ == 0) && ((direction == 0) || (direction == -1)) {
    killRegion(&prevRgn, -1);
    mergeRegions(&prevRgn, &dyingRgn);
  }
  
  dyingRgn.occ = 0;
  munmap(dyingRgn.start, dyingRgn.size);
  
  
  region *nextRgn = nextRgn.next
  if nextRgn != NULL && (prevRgn.occ == 0) && ((direction == 0) || (direction == 1)) {
    killRegion(&nextRgn, 1);
    mergeRegions(&dyingRgn, &nextRgn);
  }
  munmap(&dyingRgn, (dyingRgn.size + sizeof(dyingRgn) + sizeof(size_t)));
}

int mergeRegions(region *rgn1, region *rgn2) {
  int sizeToExpandRgn1 = sizeof(size_t) + sizeof(rgn2) + rgn2.size;
  rgn1.size = rgn1.size + sizeToExpandRgn1;
  rgn1.end = rgn2.end;
  rgn1.next = rgn2.next;
  region *nextRgn = rgn1.next;
  nextRgn.prev = &rgn1;
}

int roundUp(size_t size) 
{
  int piece = 4;
  int amountOff = size%piece;
  if (amountOff == 0) {
    return size;
  } else {
    return size + (piece - amountOff);
  }
}

