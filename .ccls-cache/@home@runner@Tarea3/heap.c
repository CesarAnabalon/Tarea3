#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"

typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;


void* heap_top(Heap* pq)
{
  if(pq->size == 0) return NULL;

  return pq->heapArray[0].data;
}

void heap_push(Heap* pq, void* data, int priority)
{
  if(pq->size == pq->capac)
  {
    int reajuste = (pq->capac*2)+1;
    pq->heapArray = (heapElem *)realloc(pq->heapArray, reajuste*sizeof(heapElem));
    pq->capac = reajuste;
  }
  
  heapElem nuevo;
  nuevo.data = data;
  nuevo.priority = priority;

  int posicion = pq->size;
  pq->heapArray[posicion] = nuevo;
  pq->size++;

  int newPos = (posicion - 1) / 2;

  while (posicion > 0 && (pq->heapArray[posicion].priority > pq->heapArray[newPos].priority)) 
  {
    heapElem temp = pq->heapArray[posicion];
    pq->heapArray[posicion] = pq->heapArray[newPos];
    pq->heapArray[newPos] = temp;

    posicion = newPos;
    newPos = (posicion - 1) / 2;
  }
}

void heap_pop(Heap* pq)
{
  if(pq->size == 0) return;

  pq->heapArray[0] = pq->heapArray[pq->size-1] ;
  pq->size--;
  int raiz = 0;

  while(1)
  {
    int izq = (2*raiz)+1;
    int der = (2*raiz)+2;
    int priority = raiz;
    
    if (izq < pq->size && (pq->heapArray[izq].priority > pq->heapArray[priority].priority))
    {
      priority = izq;
    }

    if (der < pq->size && pq->heapArray[der].priority > pq->heapArray[priority].priority) 
    {
      priority = der;
    }
    if (priority == raiz) 
    {  
      break;
    }

    heapElem temp = pq->heapArray[raiz];
    pq->heapArray[raiz] = pq->heapArray[priority];
    pq->heapArray[priority] = temp;

    raiz = priority;
  }
}

Heap* createHeap(){
  Heap* heap = (Heap*)malloc(sizeof(Heap));
  heap->heapArray = (heapElem*)malloc(3 * sizeof(heapElem));
  heap->size = 0;
  heap->capac = 3;
  return heap;
}
