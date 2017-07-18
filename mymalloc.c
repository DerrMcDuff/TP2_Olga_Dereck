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
  int nxtLinked; // si le bloc est lie a son precedant prevLinked vaut 1, sinon 0
  void* start;
  void* end;
  size_t size;
  struct bloc* nxt;
  
} region;

bloc lastBloc = NULL;
bloc lastRgn = NULL;

//les pages sont compose de 4096 alloue par mmap
void new_bloc(void* addr) {

  bloc newBloc;
  
  newBloc.occ = 1;
  newBloc.nxtLinked = 0;
  newBloc.start = mmap(addr, 4096,  PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
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

void myfree(void *ptr){
  
  startRegion
  
}

