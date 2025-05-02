#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
//#include "stdafx.h"
#include <string.h>
#include <memory.h>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <iostream>
#include "inout.h"
#include "checkFunc.h"
#include "timeexpanded.h"
#include "search.h"
#include "binaryheap.h"
#include "netbuild.h"
#include "test.h"
#include "sepsearch.h"
#include "deleteStation.h"


using namespace std;


heap_sch_struct* Init_heap(int maxNum, int flag) {
	heap_sch_struct* heap;				//IMP: need to be free

	heap = (heap_sch_struct*)malloc(sizeof(heap_sch_struct));	//size could be less than maxNum

	if (heap) 
	{
		heap->heapNode = generate_int_arr(maxNum);
		heap->heapPos = generate_int_arr(maxNum);
		heap->heapKey = generate_int_arr(maxNum);
		heap->dist = generate_int_arr(maxNum);
		heap->parent = generate_int_arr(maxNum);
		//heap->time		= generate_int_arr(maxNum);
		//heap->pred		= generate_int_arr(maxNum);
		//heap->predLink	= generate_int_arr(maxNum);
		//heap->tempTime	= generate_int_arr(maxNum);
		//heap->tempDist	= generate_int_arr(maxNum);

		if (flag == 0)
			heap->schFlag = generate_char_arr(maxNum);		//
		else
		{
			heap->distRL = generate_int_arr(maxNum);
			heap->lwbVal = generate_int_arr(maxNum);
		}
		heap->state = generate_int_arr(maxNum);
		heap->size = 0;
		return heap;
	}

}


void Destroy_heap(heap_sch_struct* heap, int flag) {
	free(heap->heapNode);
	free(heap->heapPos);
	//free(heap->time);
	//free(heap->pred);
	free(heap->state);
	//free(heap->tempTime);
	//if (flag==1){
	//free(heap->realState);
	//free(heap->realDist);
	//}

	free(heap->heapKey);
	free(heap->dist);

	free(heap);
	printf("\n Destroyed heap");
}

void resetHeap(int maxNum, heap_sch_struct* heap, int flag)
{
	heap->size = 0;
	
	for (int i = 0; i < maxNum; i++)
	{
		heap->heapNode[i] = 0;
		heap->heapPos[i] = 0;
		heap->heapKey[i] = 0;
		heap->dist[i] = 0;
		heap->state[i] = 0;
		heap->parent[i] = -1;

		if (flag == 1)
		{
			heap->distRL[i] = 0;
		}
		else if (flag == 0)
		{
			heap->schFlag[i] = 0;
		}
	}
}

void MakeHeapEmpty(heap_sch_struct* heap) {
	heap->size = 0;
}

int IsEmpty(heap_sch_struct* heap) {
	return heap->size == 0;
}

int IsFull(heap_sch_struct* heap) {
	return heap->size == heap->capacity;
}

void InsertElement(int nodeKey, int node, heap_sch_struct* heap)
//function: insert a key
{
	int	i, father;				//note the heap is indexed from 1

	i = ++heap->size;
	father = i / 2;

	heap->state[node] = 1;

	for (; father > 0 && nodeKey < heap->heapKey[father]; i /= 2, father /= 2) {
		heap->heapNode[i] = heap->heapNode[father];
		heap->heapKey[i] = heap->heapKey[father];
		heap->heapPos[heap->heapNode[i]] = i;
	}
	heap->heapNode[i] = node;
	heap->heapPos[node] = i;
	heap->heapKey[i] = nodeKey;

}


void FindMin(heap_sch_struct* heap)
{
	//function: find min key of the heap
	if (!IsEmpty(heap)) {
		heap->topNode = heap->heapNode[1];
	}
	else {
		printf("The heap is empty");
		exit(1);
	}
}


void DeleteAndExtractMin(heap_sch_struct* heap)
{
	//function: delete and extract min key of the heap

	int i=0, child=0, lastHeapPos=0;

	heap->topNode = heap->heapNode[1];
	heap->topKey = heap->heapKey[1];
	lastHeapPos = heap->size--; // size - 1

	for (i = 1; i * 2 <= heap->size; i = child) {
		child = i * 2;
		if (heap->heapKey[child] > heap->heapKey[child + 1] && child != heap->size)
			child++;
		if (heap->heapKey[lastHeapPos] > heap->heapKey[child])
		{
			heap->heapKey[i] = heap->heapKey[child];
			heap->heapNode[i] = heap->heapNode[child];
			heap->heapPos[heap->heapNode[i]] = i;
		}
		else
			break;
	}
	if (heap->size > 0)
	{
		heap->heapNode[i] = heap->heapNode[lastHeapPos];
		heap->heapKey[i] = heap->heapKey[lastHeapPos];
		heap->heapPos[heap->heapNode[i]] = i;
	}
}


void DecreaseKey(int nodeKey, int node, heap_sch_struct* heap) {
	//function: decrease key of the heap

	int i, father;

	i = heap->heapPos[node];	//get pos
	father = i / 2;

	for (; father > 0 && nodeKey < heap->heapKey[father]; i /= 2, father /= 2) {
		heap->heapNode[i] = heap->heapNode[father];
		heap->heapKey[i] = heap->heapKey[father];
		heap->heapPos[heap->heapNode[i]] = i;
	}
	heap->heapNode[i] = node;
	heap->heapPos[node] = i;
	heap->heapKey[i] = nodeKey;
}