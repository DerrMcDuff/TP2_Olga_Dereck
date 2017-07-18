// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit ??tre dans ce fichier

#include <stdio.h>
#include <sys/mman.h>
#include <math.h>
#include "mymalloc.h"




bloc *lastbloc = NULL;
region *lastRgn = NULL;
region *firstRgn = NULL;

//les pages sont compose de 4096 alloue par mmap
void new_bloc(void* addr) {

  bloc * newbloc = mmap(addr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  
  newbloc->occ = 1;
  newbloc->nxtLinked = 0;
  newbloc->start = &newbloc;
  newbloc->end = newbloc->start + 4096;
  newbloc->size = 4096 - (sizeof(size_t) + sizeof(bloc));
  newbloc->nxt = NULL;
  if(lastbloc != NULL){
    lastbloc->nxt = newbloc;
  }

  lastbloc = newbloc;

}



//chaque page va contenir plusieurs blocs
void * new_rgn(size_t size, void* addr) {
  
  if (firstRgn == NULL) {
    firstRgn = addr;
  }
  
  region *newRgn = addr;
  
  newRgn->occ = 1;
  newRgn->start = addr;
  newRgn->end = addr + size;
  newRgn->size = size - (sizeof(size_t) + sizeof(region));
  newRgn->prev = lastRgn;
  newRgn->nxt = NULL;
  if(lastRgn != NULL){
    lastRgn->nxt = newRgn;
  }
  
  lastRgn = newRgn;
  return newRgn;
}



  void *mymalloc(size_t size){


    region *rgnToUse;
    int sizeRoundUp = roundUp(size + sizeof(size_t) + sizeof(bloc));

    //Si aucun bloc n'a encore ete initialisee, on en cree un
    if(lastbloc == NULL) {

      new_bloc(NULL);
      new_rgn(sizeRoundUp + sizeof(region), lastbloc->start);

    }

    //Si la taille demandee est plus grande que 4ko, on va devoir allouer plusieurs blocs
    if (size > 4096) {
      
      printf("Warning: Memory requested is high");
      
      int nbB = ceil(size/4096);

      // creation du prochain bloc, s'il n'est pas encore cree
      if(lastbloc != NULL){
        new_bloc(lastbloc->end);
      }

      // creation de la region avec l'addresse du debut de la premiere page, et la taille donnee 
      rgnToUse = new_rgn(sizeRoundUp, lastbloc->start);
      

      for(int i = 1; i<nbB; i++){
        new_bloc(lastbloc->end);
        lastbloc->nxtLinked = 1;
      }


      //Si il reste de la place dans le dernier bloc, on prends cet espace et on fait de lui
      //une region qui n'est pas occupee (occ=0)
      if(lastRgn->size != 0){
        createLastReg(rgnToUse, (lastbloc->size - rgnToUse->size));
      }

    } else {
      //Sinon on va parcourir les regions une par une afin de voir si
      //une d'entre elles est libre et a assez d'espace pour pouvoir etre retournee
      rgnToUse = firstRgn->start;
      while ((rgnToUse == NULL) && (rgnToUse->occ == 1) && (rgnToUse->size < sizeRoundUp)){

        region *nxt = rgnToUse->nxt;
        rgnToUse = nxt;

      }

      if (lastRgn->size < sizeRoundUp) {

        new_bloc(lastbloc.end);
        rgnToUse = new_rgn(size, lastbloc.start);
        createLastReg(rgnToUse, (lastbloc.size - rgnToUse.size) );

      } else {

        rgnToUse = new_rgn(size, lastRgn.start);
        createLastReg(rgnToUse, (lastbloc.size - rgnToUse.size) );
      }



    }

    return rgnToUse;
  }


  region *createLastReg(region *reg, int size) {

    region last = new_rgn(size, reg.end);
    last.occ = 0;
    return last;
    
  }
  
  void *mymalloc(size_t size) {


    region *rgnToUse;
    int sizeRoundUp = roundUp(size + sizeof(size_t) + sizeof(bloc));

    //Si aucun bloc n'a encore ete initialisee, on en cree un
    if(lastbloc == NULL) {

      new_bloc(NULL);
      new_rgn(sizeRoundUp + sizeof(rgn), lastbloc.start);
      
    }

    //Si la taille demandee est plus grande que 4ko, on va devoir allouer plusieurs blocs
    if (size > 4096) {

      int nbB = ceil(size/4096);

      //creation du prochain bloc, s'il n'est pas encore cree
      if(lastbloc != NULL){
        new_bloc(lastbloc.end);
      }

      // creation de la region avec l'addresse du debut de la premiere page, et la taille donnee
      rgnToUse = new_rgn(sizeRoundUp, lastbloc.start);

      for(int i = 1; i<nbB; i++){
        new_bloc(lastbloc.end);
        lastbloc.nxtLinked = 1;
      }


      //Si il reste de la place dans le dernier bloc, on prends cet espace et on fait de lui
      //une region qui n'est pas occupee (occ=0)
      if(lastRgn.size != 0){
        new_rgn((lastbloc.size - lastRgn.size), lastRgn.nxt);
        createLastReg(rgnToUse, (lastbloc.size - rgnToUse.size) );
      }


    } else{
      //Sinon on va parcourir les regions une par une afin de voir si
      //une d'entre elles est libre et a assez d'espace pour pouvoir etre retournee
      rgnToUse = firstregion.start;
      while ((rgnToUse.occ == 1) && (rgnToUse.size < sizeRoundUp)){

        region nxt = rgnToUse.nxt;
        rgnToUse = nxt;

      }

      if (lastRgn.size < sizeRoundUp) {

        new_bloc(lastbloc.end);
        rgnToUse = new_rgn(size, lastbloc.start);
        createLastReg(rgnToUse, (lastbloc.size - rgnToUse.size) );

      } else {

        rgnToUse = new_rgn(size, lastRgn.start);
        createLastReg(rgnToUse, (lastbloc.size - rgnToUse.size) );
      }



    }

    return rgnToUse;
  }


  region createLastReg(region reg, int size){

    region last = new_rgn(size, reg.end);
    last.occ = 0;
    return last;

  }





// Objects are freed from memory with this incantation
void myfree(void *ptr) {
  
  // Make a region pointer out of the void pointer! 
  region *focusedregion = ptr;
  
  if ((focusedregion != NULL) && (focusedregion.occ == 1)) {
    // region to free found
    freeregion(&focusedregion, 0);
  } else if ((focusedregion != NULL) && (focusedregion.occ == 0)) {
    // Turns out the memory is already freed! Good on you! 
    printf("Already freed");
  } else {
    // Hu Oh 
    printf("This was not an accessible object");
  }
  
}

void freeregion(region *dyingRgn, int direction) {
  
  // Let's see if we can merge this soon-to-be free region with the one before...
  region *prevRgn = dyingRgn.prev;
  if prevRgn != NULL && (prevRgn.occ == 0) && ((direction == 0) || (direction == -1)) {
    freeregion(&prevRgn, -1);
    mergeregions(&prevRgn, &dyingRgn);
  }
  
  dyingRgn.occ = 0;
  
   // Let's see if we can merge this soon-to-be free region with the one after...
  region *nxtRgn = nxtRgn.nxt
  if nxtRgn != NULL && (prevRgn.occ == 0) && ((direction == 0) || (direction == 1)) {
    freeregion(&nxtRgn, 1);
    mergeregions(&dyingRgn, &nxtRgn);
  }
  
  munmap(&dyingRgn, (dyingRgn.size + sizeof(dyingRgn) + sizeof(size_t)));
}

void mergeregions(region *rgn1, region *rgn2) {
  int sizeToExpandRgn1 = sizeof(size_t) + sizeof(rgn2) + rgn2.size;
  rgn1.size = rgn1.size + sizeToExpandRgn1;
  rgn1.end = rgn2.end;
  rgn1.nxt = rgn2.nxt;
  region *nxtRgn = rgn1.nxt;
  nxtRgn.prev = &rgn1;
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

