/*
Tuesday, August 1, 2023 - cache.c
- Defines cache functions and print function
- Defines cachelines and cache structures
*/
#include "cache.h"

extern cacheline* cache[CACHE_SIZE];  // bring in external cache line
extern mem memory[MEM_SIZE];
extern unsigned char cacheMode, cpu_time, replPol;

/* Creates a New Cache Line */
cacheline* createCacheline(unsigned short address, unsigned short data){
  cacheline* newLine = malloc(sizeof(cacheline));
  word_byte* new = malloc(sizeof(word_byte));

  new->word = data;
  newLine->address = address;
  newLine->data = new;
  newLine->age = 0;
  newLine->dirtyBit = 0;
  
  return newLine;
}

/* Cache Bus */
void cacheBus(unsigned short addr, unsigned short *data, char readORwrite, char wordORbyte){
  int vacancy, index, searchResult = searchCache(addr);
  switch (searchResult == -1) {
    case HIT:
      ageCache(cache[searchResult]->age);                       // Age all younger cachelines
      cache[searchResult]->age = 0;                             // Reset hit line's age
      index = (searchResult == -1) ? LSN(addr) : searchResult;  // Set Cache index based on hit/miss

      if(readORwrite) {
        cache[index]->data->word = *data;                       // WRITE: Save data passed to cacheline

        if(!replPol && readORwrite) {
          bus(addr, data, WRITE, WORD_);              // WRITE - WT: Save data passed to mem if write through is selected
        }
        else{
          cache[index]->dirtyBit = 1;                        // WRITE - WB: Set Dirty Bit
        }
      } else{
        *data = cache[index]->data->word;                       // READ: Save value of hit line to data
      }
    break;

    case MISS:
      ageCache(CACHE_SIZE);                                       // Age all cache lines
      if(!readORwrite){
        bus(addr, data, READ, wordORbyte);
      }
      if(readORwrite && !replPol){
        bus(addr, data, WRITE, wordORbyte); // Read/Write data to/from memory[addr]
      }
      if(cacheMode){
      /* Associative */
        vacancy = searchForVacantLine();                          // Search for Empty Line [Line ID if vacancy | -1 if full]
        if(vacancy >= 0){
          cache[vacancy] = createCacheline(addr, *data);          // Save new line to vacant line
        } else{
          cache[removeOldest()] = createCacheline(addr, *data);   // Replace oldest line
        }
      } else{
        /* Direct */
        removeCacheLine(LSN(addr));                               // Remove Line if Needed (Check is done in function)
        cache[LSN(addr)] = createCacheline(addr, *data);          // Define New Cacheline
      }
    break;

    /* Unknown Bus State */
    default: printf("Error: Unknown Cache Bus State\n"); break;
  }
  return;
}


/* Age Lines in Cache */
void ageCache(unsigned int maxAge){
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] != NULL && cache[i]->age < maxAge) cache[i]->age++;
  }
}

/* Search Cache for Address 
- Returns cache line # for a hit (0 to CACHE_SIZE)
- Returns -1 for a miss
*/
int searchCache(unsigned short address){
  switch(cacheMode){
    case ASC:
      /* Associative */
      for(int i = 0; i < CACHE_SIZE; i++)
        if(cache[i] != NULL && cache[i]->address == address) return i; // Return index if hit
      return -1;       // Return -1 for miss
    break;
    case DIR:
      /* Direct */
      if(cache[LSN(address)] != NULL && cache[LSN(address)]->address == address) return LSN(address); // Return LSN for hit
      else return -1;  // Return -1 for miss
    break;
  }
  return -1;
}

/* Remove the Least Recently Used Cache Line from the Cache 
- Returns oldest cacheline ID (after it is removed)
*/
int removeOldest(){
  int oldestAge = 0, oldestIndex;

  /* Cycle through cache, searching for oldest */
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] != NULL && cache[i]->age > oldestAge){
      oldestAge = cache[i]->age;            // Update oldest age
      oldestIndex = i;                      // Update oldest index
    }
  }

  removeCacheLine(oldestIndex);               // Remove cacheline
  return oldestIndex;                         // Return index of oldest line
}

/* Removes a Cache Line */
void removeCacheLine(int lineID){
  /* Check cache line exists */
  if(cache[lineID] == NULL) return;

  /* Remove Line */
  if(cache[lineID]->dirtyBit && replPol == WB){
    memory->word[cache[lineID]->address >> 1] = cache[lineID]->data->word;
  }
  free(cache[lineID]);   // Free oldest cache line
}

/* Prints Cache */
void printCache(){
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] != NULL){
      printf("%2d: ", i);                             // Print Line ID
      if(replPol) printf("[%d]", cache[i]->dirtyBit); // Print dirtyBit (if replacement policy is WB)
      printf("[%04X][%04X]", cache[i]->address, cache[i]->data->word); // Print Line
      if(cacheMode) printf("[%X]", cache[i]->age);    // Print age (if using associative mapping)
    }
    else{
      printf("%2d: ", i);                             // Print Line ID
      if(replPol) printf("[-]");                      // Print empty dirtyBit (if replacement policy is WB)
      printf("[----][----]");                      // Print empty Line
      if(cacheMode) printf("[-]");                    // Print empty age (if using associative mapping)
    }
    printf("\n");
  }
}

/* Search for Empty Cache Line
- Returns line ID if vacancy is found
- Returns -1 if cache is full
*/
int searchForVacantLine(){
  for(int i = 0; i < CACHE_SIZE; i++){
    if(cache[i] == NULL) return i; // Vacancy
  }
  return -1; // No Vacancy
}

