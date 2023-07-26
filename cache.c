/*
Wednesday, July 26, 2023 - cache.c
- Defines cache functions and print function
- Defines cachelines and cache structures
*/
#include "cache.h"

extern cacheline* cache[CACHE_SIZE];  // bring in external cache line
extern mem memory[MEM_SIZE];
extern int cacheMode, cpu_time;

/* Creates a New Cache Line */
cacheline* createCacheline(unsigned short address, unsigned short data){
  cacheline* newLine = malloc(sizeof(cacheline));
  word_byte* new = malloc(sizeof(word_byte));
  new->word = data;

  newLine->age = 0;
  newLine->address = address;
  newLine->dirtyBit = 0;
  newLine->data = new;
  
  return newLine;
}

/* Sends a Cache Line 
- If HIT: ages younger cache lines, resets age, and returns line #
- If MISS: adds new address to the cache, ages all other lines, and returns line #
*/
int sendCacheLine(unsigned short address){
  /* Check for hit/miss */
  int searchResult = searchCache(address);
  //printf("searchResult = %d\n", searchResult);
  /* Execute Based on Hit/Miss */
  if(searchResult >= 0){
  /* Hit */
    switch(cacheMode){
      case ASC:
        ageCache(cache[searchResult]->age); // Age all cache lines younger than hit line
        cache[searchResult]->age = 0;       // Reset line's age 
      break;
      case DIR: break;
      case HYB:
      /* TBD */
      break;
    }
    return searchResult; /* exit - hit: return line # of hit */
  }

  /* Miss */
  cacheline* new = createCacheline(address, memory->word[address >> 1]);
  cpu_time += 2;

  switch(cacheMode){
    case ASC:
      /* Associative */
      /* Check for Vacant Cache Line */
      
      for(int i = 0; i < CACHE_SIZE; i++){
        if(cache[i] == NULL){
          ageCache(CACHE_SIZE);   // Age all cache lines
          cache[i] = new;         // Save new line to cache
          return i; /* exit - miss, vacant cache */
        }
      }

      /* Full Cache */
      int oldestIndex = removeOldest();
      ageCache(CACHE_SIZE);       // Age all cache lines
      cache[oldestIndex] = new;   // Save new line to cache
      return oldestIndex; /* exit - miss, full cache */
    break;
    case DIR:
      /* Direct */
      //printf("Direct Cache Mode\n");
      cache[LSN(address)] = new;
      return LSN(address);
    break;
    case HYB:
      /* Hybrid */
      return -1;
    break;
  }  
  return -1;
}


/* Age Lines in Cache */
void ageCache(unsigned int maxAge){
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] != NULL) {
      if(cache[i]->age < maxAge) cache[i]->age++;
    }
  }
}

/* Search Cache for Address 
- Returns cache line # for a hit (0 to CACHE_SIZE)
- Returns -1 for a miss
*/
int searchCache(unsigned short address){
  //printf("searching cache for %04X using mode #%c\n", address, cacheMode);
  switch(cacheMode){
    case ASC:
      /* Associative */
      //printf("searching ASC\n");
      for(int i = 0; i < CACHE_SIZE; i++){
        if(cache[i] != NULL){
          //printf("cache[%d]->address = %04X and cache[%d]->data = %04X\n", i, cache[i]->address, i, cache[i]->data->word);
          if(cache[i]->address == address){
            //printf("Hit at cache line #%d\n", i);
          return i;
          }
      }
    }
    //printf("Miss :(\n");
    return -1; // miss
    break;
    case DIR:
      /* Direct */
      if(cache[LSN(address)] != NULL && cache[LSN(address)]->address == address){
        return LSN(address);  // if address in location with = LSN then return hit
      }
      else return -1; // miss
    break;
    case HYB:
      /* Hybrid */
      return -1;
    break;
  }
  return -1;
}

/* Remove the Least Recently Used Cache Line from the Cache */
int removeOldest(){
  int oldestAge = 0, oldestIndex = 0;

  /* Cycle through cache, searching for oldest */
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] != NULL){
      if(cache[i]->age > oldestAge){
        oldestAge = cache[i]->age;
        oldestIndex = i;
      }
    }
  }
  if(cache[oldestIndex]->dirtyBit){
    bus(cache[oldestIndex]->address, &cache[oldestIndex]->data->word, WRITE, WORD);
  }
  free(cache[oldestIndex]);   // Free oldest cache lines #moved here from sendCacheLine()
  printf("Removed cache line #%d\n", oldestIndex);
  return oldestIndex;
}

/* Prints Cache */
void printCache(){
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] != NULL){
      printf("%2d: [%d][%04X][%04X]", i, cache[i]->dirtyBit, cache[i]->address, cache[i]->data->word);
      if(cacheMode != '2') printf("[%X]", cache[i]->age);
    }
    else{
      printf("%2d: [-][----][----]", i);
      if(cacheMode != '2') printf("[-]");
    }
    printf("\n");
  }
}