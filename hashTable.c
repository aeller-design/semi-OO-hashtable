#include <limits.h>
#include "hashTable.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
   HTEntry *entry;
   struct node *next;
} HashNode;

typedef struct
{
   HTFunctions *functions;
   unsigned *sizes, sizeIndex;
   unsigned totalEntries, uniqueEntries;
   int numSizes;
   float rehashLoadFactor;
   HashNode **arr;
   
} HashTable;

void mallocError()
{
   fprintf(stderr, "malloc failure in %s at %d\n",__FILE__, __LINE__);
   exit(EXIT_FAILURE);
}

void freeNode(HashNode *node, FNDestroy destroy)
{
   if(destroy != NULL)
      destroy(node->entry->data);
   free(node->entry->data);
   free(node->entry);
   free(node);
}

void traverseLinksDes(HashNode* hashNodeP, FNDestroy destroy)
{
   HashNode *hashNodeQ;

   while((hashNodeP)!= NULL)
   {
      hashNodeQ = hashNodeP->next;
      freeNode(hashNodeP, destroy);
      hashNodeP = hashNodeQ;
   }
}   

void checkRoot(HashNode* htRootNode, FNDestroy destroy, int i)
{
   HashNode *hashNodeP;

   if (htRootNode != NULL)
   { 
      hashNodeP = htRootNode;
      traverseLinksDes(hashNodeP, destroy);
   }   
}   

void destroyArr(HashNode **arr, unsigned size, FNDestroy destroy)
{
   int i;
   for(i = 0; i < size; i++)
      checkRoot(arr[i], destroy, i);
   free(arr);
}

unsigned getHash(void *hashTable, void *data)
{
   return ((HashTable*)hashTable)->functions->hash(data) 
      % htCapacity(hashTable);
}

void traverseLinks(HashNode *listNode, HashNode *newArrNode)
{
   HashNode* node = newArrNode;
   while(node->next)
      node = node->next;

   node->next = listNode;   
   node->next->next = NULL;
}
void moveNode(void* hashTable, HashNode *listNode, HashNode **newArr)
{
   
   unsigned hash = getHash(hashTable,((HashNode*)listNode)->entry->data);
   if (newArr[hash])
      traverseLinks(listNode, newArr[hash]);
   else   
   {   
      newArr[hash] = listNode;
      newArr[hash]->next = NULL;
   }   
}

void checkNodes(void* hashTable, HashNode* node, HashNode **newArr)
{
   HashNode *listNode, *nextNode;

   listNode = node;
   while(listNode)
   {
      nextNode = listNode->next;
      moveNode(hashTable, listNode, newArr);
      listNode = nextNode;
   }     
}

void checkIndex(void* hashTable, HashNode* node, HashNode **newArr, int i)
{
   if (node)
      checkNodes(hashTable, node, newArr);
}

void rePopulate(void* hashTable, HashNode **newArr)
{
   int i;

   for(i = 0; i < ((HashTable*)hashTable)->sizes[
      ((HashTable*)hashTable)->sizeIndex - 1]; i++)
   {
      checkIndex(hashTable,((HashTable*)hashTable)->arr[i],newArr,i);
   }   
}   
void rehash(void *hashTable, void *data)
{
   HashNode **newArr;
   (((HashTable*)hashTable)->sizeIndex)++; 
   newArr = calloc(
      ((HashTable*)hashTable)->sizes[((HashTable*)hashTable)->sizeIndex], 
      sizeof(HashNode*));
   if(newArr  == NULL)
      mallocError();
   rePopulate(hashTable, newArr);   
   free(((HashTable*)hashTable)->arr);
   
   ((HashTable*)hashTable)->arr = newArr;
}   
float calcLf(void *hashTable)
{
   return ((float)htUniqueEntries(hashTable)) /
      ((float)(((HashTable*)hashTable)->
      sizes[((HashTable*)hashTable)->sizeIndex]));
}   
void checkRehash(void *hashTable, void *data)
{
   float lf = calcLf(hashTable); 

   if((((HashTable*)hashTable)->numSizes > 
      (((HashTable*)hashTable)->sizeIndex + 1)) &&
      (((HashTable*)hashTable)->rehashLoadFactor < lf) &
      (((HashTable*)hashTable)->rehashLoadFactor!=1))
      rehash(hashTable, data);
}

void entryCount(void *hashTable, unsigned tot, unsigned unq)
{
   ((HashTable*)hashTable)->totalEntries += tot;
   ((HashTable*)hashTable)->uniqueEntries += unq;
}

unsigned dataCompare(void *hashTable, void *data1, void *data2)
{
   return ((HashTable*)hashTable)->functions->compare(data1, data2);
}

unsigned checkDuplicate(
   void *hashTable,
   HashNode *listNode,
   void *data)
{
   if(dataCompare(hashTable, data,
      listNode->entry->data )==0) 
   {
      entryCount(hashTable, 1, 0);
      listNode->entry->frequency++;
      return listNode->entry->frequency;
   }   
   return 0;
}   

HashNode* initDataNode(void *data)
{
   HashNode *dataNode;
   dataNode = (HashNode*)malloc(sizeof(HashNode));
   dataNode->next = NULL;
   dataNode->entry = (HTEntry*)malloc(sizeof(HTEntry));
   dataNode->entry->data = data;
   dataNode->entry->frequency = 1;
   return dataNode;
}

void cpyArr(int numSizes, unsigned *src, unsigned *dest)
{
   int i;
   for(i = 0; i < numSizes; i++)
      src[i] = dest[i];   
}

int testAscSize(int numSizes, unsigned *sizes)
{
   int i;

   for(i = 0; i < numSizes-1; i++)
      if(sizes[i] >= sizes[i+1])
         return 0;

   return 1;      
}   
void asserts(int numSizes, unsigned *sizes, float rehashLoadFactor)
{
   assert(numSizes >= 1);
   if (numSizes > 1)
      assert(testAscSize(numSizes, sizes));
   assert((rehashLoadFactor > 0) && (rehashLoadFactor <= 1));
   assert(sizes[0] != 0);
}

void cpyFunctions(HashTable *hashTable, HTFunctions *functions)
{
   if((hashTable->functions = malloc(sizeof(HTFunctions)))== NULL)
      mallocError();
   *(hashTable->functions) = *functions;   
}

void* htCreate(
   HTFunctions *functions,
   unsigned sizes[],
   int numSizes,
   float rehashLoadFactor)
{
   HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
   if(ht == NULL)
      mallocError();
      
   asserts(numSizes, sizes, rehashLoadFactor);

   ht->functions = functions;
   ht->numSizes = numSizes;
   ht->rehashLoadFactor = rehashLoadFactor;
   ht->totalEntries = 0;
   ht->uniqueEntries = 0;
   ht->sizeIndex = 0;

   if((ht->sizes = (unsigned*)malloc(numSizes * sizeof(unsigned)))==NULL)
      mallocError();

   cpyArr(numSizes, ht->sizes, sizes);

   ht->arr = calloc(sizes[ht->sizeIndex], sizeof(HashNode*));
   if(ht->arr == NULL)
      mallocError();

   return ht;
}

void htDestroy(void *hashTable)
{
   destroyArr(
      ((HashTable*)hashTable)->arr, 
      ((HashTable*)hashTable)->sizes[((HashTable*)hashTable)->sizeIndex],
      ((HashTable*)hashTable)->functions->destroy);
   free(((HashTable*)hashTable)->sizes);
   free((HashTable*)hashTable);
}

unsigned htAdd(void *hashTable, void *data)
{
   unsigned hash;
   unsigned freq = 0;
   HashNode *listNode, *dataNode;
   HashNode *prevNode = NULL;

   assert(data);

   checkRehash(hashTable, data);

   hash = getHash(hashTable, data);
   listNode = ((HashTable*)hashTable)->arr[hash];
   while(listNode)
   {  
      freq += checkDuplicate(hashTable, listNode, data);
      prevNode = listNode;
      listNode = listNode->next;
   }   
   if(freq > 1)
      return freq;
   dataNode = initDataNode(data);
   if(prevNode)
      prevNode->next = dataNode;
   else
      ((HashTable*)hashTable)->arr[hash] = dataNode;
   entryCount(hashTable, 1, 1);
   return 1;
}

void setEntry(HTEntry *entry, HashNode *listNode)
{
   entry->data = listNode->entry->data;
   entry->frequency = listNode->entry->frequency;
}

void compareEntries(void* hashTable, HTEntry *entry, 
   HashNode *listNode, void* data)
{
   if (!(dataCompare(hashTable, listNode->entry->data, data))) 
      setEntry(entry, listNode);   
}

void searchLinks(void* hashTable, HTEntry *entry, 
   HashNode *listNode, void* data)
{
   do{ 
      compareEntries(hashTable, entry, listNode, data);      
      listNode = listNode->next;
   } while(listNode);
}

HTEntry htLookUp(void *hashTable, void *data)
{
   HashNode *listNode;
   HTEntry entry;
   entry.data = NULL;
   entry.frequency = 0;

   assert(data != NULL);
   listNode = ((HashTable*)hashTable)->arr[getHash(hashTable, data)];
   
   if(!listNode)
      return entry;

   searchLinks(hashTable, &entry, listNode, data);

   return entry;
}

void copyEntry(HTEntry *dest, HTEntry src)
{
   dest->frequency = src.frequency;
   dest->data = src.data;
}   

void checkLinks2(HashNode* node, HTEntry* entryArr, int *j)
{
   HashNode *listNode = node;
   while(listNode)
   {
      copyEntry(&entryArr[(*j)], *(listNode->entry));
      (*j)++;
      listNode = listNode->next;
   }     
}

void checkIndex2(HashNode* node, HTEntry* entryArr, int *j)
{
   if (node)
      checkLinks2(node, entryArr, j);
}

void scanArr(HTEntry *entryArr, void *hashTable)
{
   int i=0;
   int j=0;
   
   while(j < htUniqueEntries(hashTable) && i < htCapacity(hashTable))
   {
      checkIndex2(((HashTable*)hashTable)->arr[i],entryArr, &j);
      i++;
   }   
}

HTEntry* htToArray(void *hashTable, unsigned *size)
{
   HTEntry *entryArr;
   *size = htUniqueEntries(hashTable);
   if(!(*size))
      return NULL;
   
   if(!(entryArr = (HTEntry*)malloc((*size)* sizeof(HTEntry))))
      mallocError();
   scanArr(entryArr, hashTable); 
   
   return entryArr; 
}

unsigned htCapacity(void *hashTable)
{
   return ((HashTable*)hashTable)->
      sizes[((HashTable*)hashTable)->sizeIndex];
}

unsigned htUniqueEntries(void *hashTable)
{
   return ((HashTable*)hashTable)->uniqueEntries;
}

unsigned htTotalEntries(void *hashTable)
{
   return ((HashTable*)hashTable)->totalEntries;
}

void mCheckNodes(HashNode* node, HTMetrics *metrics)
{
   unsigned clTemp = 1;
   HashNode *listNode;

   listNode = node;
   (metrics->numberOfChains)++;

   while(listNode->next)
   {
      clTemp++;
      listNode = listNode->next;
   }

   if(clTemp > (metrics->maxChainLength))
      metrics->maxChainLength = clTemp;
}

void mCheckIndex(HashNode *node, HTMetrics *metrics)
{
   if (node)
      mCheckNodes(node, metrics);
}

void mTraverseTable(void* hashTable, HTMetrics *metrics)
{
   unsigned i;

   for(i = 0; i < htCapacity(hashTable); i++)
      mCheckIndex(((HashTable*)hashTable)->arr[i], metrics);
}   
HTMetrics htMetrics(void *hashTable)
{
   HTMetrics metrics;

   metrics.maxChainLength = 0;
   metrics.numberOfChains = 0;
   metrics.avgChainLength = 0;

   mTraverseTable(hashTable, &metrics); 
   metrics.avgChainLength = ((float)htUniqueEntries(hashTable)/
      (float)(metrics.numberOfChains));

   return metrics;
}
