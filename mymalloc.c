// Vous devez modifier ce fichier pour le TP 2
// Tous votre code doit ??tre dans ce fichier

#include <stdio.h>
#include <sys/mman.h>
#include <math.h>
#include "mymalloc.h"




bloc *lastBloc = NULL;
bloc *lastRgn = NULL;

//les pages sont compose de 4096 alloue par mmap
void new_bloc(void* addr) {

  bloc newBloc;
  
  newBloc.occ = 1;
  newBloc.nxtLinked = 0;
  newBloc.start = mmap(addr, 4096, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  newBloc.end = newBloc.start + 4096;
  newBloc.size = 4096 - (sizeof(size_t) + sizeof(bloc));
  newBloc.nxt = NULL;
  if(lastBloc != NULL){
    lastBloc->nxt = &newBloc;
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
  newRgn.prev = lastRgn;
  newRgn.nxt = NULL;
  if(lastRgn != NULL){
    lastRgn.nxt = newRgn;
  }
  
  lastRgn = newRgn;
}



  void *mymalloc(size_t size){


    region *rgnToUse;
    int sizeRoundUp = roundUp(size + sizeof(size_t) + sizeof(bloc));

    //Si aucun bloc n'a encore ete initialisee, on en cree un
    if(lastBloc == NULL) {

      new_bloc(NULL);
      new_rgn(sizeRoundUp + sizeof(region), lastBloc.start);

    }

    //Si la taille demandee est plus grande que 4ko, on va devoir allouer plusieurs blocs
    if (size > 4096) {
      
      printf("Warning: Memory requested is high")
      
      int nbB = ceil(size/4096);

      // creation du prochain bloc, s'il n'est pas encore cree
      if(lastBloc != NULL){
        new_bloc(lastBloc.end);
      }

      // creation de la region avec l'addresse du debut de la premiere page, et la taille donnee
      rgnToUse = new_rgn(sizeRoundUp, lastBloc.start);

      for(int i = 1; i<nbB; i++){
        new_bloc(lastBloc.end);
        lastBloc.nxtLinked = 1;
      }


      //Si il reste de la place dans le dernier bloc, on prends cet espace et on fait de lui
      //une region qui n'est pas occupee (occ=0)
      if(lastRgn.size != 0){
        createLastReg(rgnToUse, (lastBloc.size - rgnToUse.size));
      }

    } else {
      //Sinon on va parcourir les regions une par une afin de voir si
      //une d'entre elles est libre et a assez d'espace pour pouvoir etre retournee
      rgnToUse = firstRegion.start;
      while ((rgnToUse == NULL) && (rgnToUse.occ == 1) && (rgnToUse.size < sizeRoundUp)){

        region nxt = rgnToUse.nxt;
        rgnToUse = nxt;

      }

      if (lastRgn.size < sizeRoundUp) {

        new_bloc(lastBloc.end);
        rgnToUse = new_rgn(size, lastBloc.start);
        createLastReg(rgnToUse, (lastBloc.size - rgnToUse.size) );

      } else {

        rgnToUse = new_rgn(size, lastRgn.start);
        createLastReg(rgnToUse, (lastBloc.size - rgnToUse.size) );
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
    if(lastBloc == NULL) {

      new_bloc(NULL);
      new_rgn(sizeRoundUp + sizeof(rgn), lastBloc.start);

    }

    //Si la taille demandee est plus grande que 4ko, on va devoir allouer plusieurs blocs
    if (size > 4096) {

      int nbB = ceil(size/4096);

      //creation du prochain bloc, s'il n'est pas encore cree
      if(lastBloc != NULL){
        new_bloc(lastBloc.end);
      }

      // creation de la region avec l'addresse du debut de la premiere page, et la taille donnee
      rgnToUse = new_rgn(sizeRoundUp, lastBloc.start);

      for(int i = 1; i<nbB; i++){
        new_bloc(lastBloc.end);
        lastBloc.nxtLinked = 1;
      }


      //Si il reste de la place dans le dernier bloc, on prends cet espace et on fait de lui
      //une region qui n'est pas occupee (occ=0)
      if(lastRgn.size != 0){
        new_rgn((lastBloc.size - lastRgn.size), lastRgn.nxt);
        createLastReg(rgnToUse, (lastBloc.size - rgnToUse.size) );
      }


    } else{
      //Sinon on va parcourir les regions une par une afin de voir si
      //une d'entre elles est libre et a assez d'espace pour pouvoir etre retournee
      rgnToUse = firstRegion.start;
      while ((rgnToUse.occ == 1) && (rgnToUse.size < sizeRoundUp)){

        region nxt = rgnToUse.nxt;
        rgnToUse = nxt;

      }

      if (lastRgn.size < sizeRoundUp) {

        new_bloc(lastBloc.end);
        rgnToUse = new_rgn(size, lastBloc.start);
        createLastReg(rgnToUse, (lastBloc.size - rgnToUse.size) );

      } else {

        rgnToUse = new_rgn(size, lastRgn.start);
        createLastReg(rgnToUse, (lastBloc.size - rgnToUse.size) );
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
  region *focusedRegion = ptr;
  
  if ((focusedRegion != NULL) && (focusedRegion.occ == 1)) {
    // Region to free found
    freeRegion(&focusedRegion, 0);
  } else if ((focusedRegion != NULL) && (focusedRegion.occ == 0)) {
    // Turns out the memory is already freed! Good on you! 
    printf("Already freed");
  } else {
    // Hu Oh 
    printf("This was not an accessible object");
  }
  
}

void freeRegion(region *dyingRgn, int direction) {
  
  // Let's see if we can merge this soon-to-be free region with the one before...
  region *prevRgn = dyingRgn.prev;
  if prevRgn != NULL && (prevRgn.occ == 0) && ((direction == 0) || (direction == -1)) {
    freeRegion(&prevRgn, -1);
    mergeRegions(&prevRgn, &dyingRgn);
  }
  
  dyingRgn.occ = 0;
  
   // Let's see if we can merge this soon-to-be free region with the one after...
  region *nxtRgn = nxtRgn.nxt
  if nxtRgn != NULL && (prevRgn.occ == 0) && ((direction == 0) || (direction == 1)) {
    freeRegion(&nxtRgn, 1);
    mergeRegions(&dyingRgn, &nxtRgn);
  }
  
  munmap(&dyingRgn, (dyingRgn.size + sizeof(dyingRgn) + sizeof(size_t)));
}

void mergeRegions(region *rgn1, region *rgn2) {
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

