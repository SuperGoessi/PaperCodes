#pragma once
#pragma once
typedef struct {
	int		size;
	int		capacity;
	int		topNode;
	int		topKey;
	int		schCnt;			//search planar PPA

	int*	heapNode;
	int*	heapPos;
	int*	heapKey;
	//int	*time;
	//int	*pred;
	//int	*predLink;
	int*	dist;
	int*	distRL;
	int*	lwbVal;
	int*	state;
	char*	schFlag;		//search fronter space
	//char  *realState;
	//int	*tempDist;
	int	*tempTime;
	//int	*realDist;
	//bool* trsFlag;   // if search to this station, transfer time + 1
	int* parent;
} heap_sch_struct;	//2018

extern heap_sch_struct* sp_PT_heap_F;
extern heap_sch_struct* sp_PT_heap_B;
extern heap_sch_struct* sp_PV1_heap_F;
extern heap_sch_struct* sp_PV1_heap_B;

extern heap_sch_struct* sp_PV_heap_pre_planar_1;
extern heap_sch_struct* sp_PV_heap_pre_planar_2;
extern heap_sch_struct* sp_PV_heap_pre_original;
extern heap_sch_struct* sp_PT_heap_pre_time;
extern heap_sch_struct* sp_PT_heap_pre_transfer;

extern heap_sch_struct* sp_PV_heap_single_F;
extern heap_sch_struct* sp_PV_heap_single_B;


//declare functions below
heap_sch_struct* Init_heap(int maxNum, int flag);

void	Destroy_heap(heap_sch_struct* heap, int flag);

void	MakeHeapEmpty(heap_sch_struct* heap);

void resetHeap(int maxNum, heap_sch_struct* heap, int flag);

int		IsEmpty(heap_sch_struct* heap);

int		IsFull(heap_sch_struct* heap);

void	InsertElement(int nodeKey, int node, heap_sch_struct* heap);

void	FindMin(heap_sch_struct* heap);

void	DeleteAndExtractMin(heap_sch_struct* heap);

void	DecreaseKey(int nodeKey, int node, heap_sch_struct* heap);