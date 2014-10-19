#include <stdlib.h>
#include "heapindex.h"
#define maxheap 200


//to hold the number of items in the queue
static int R;

//create an array of Items 
static Item heap[maxheap+1];


//exchange Items in the pq
void heapexch(int i, int j)
{

	Item t,t2; //create 2 temporary Items 
	t = heap[i];  
	t2 = heap[j];
		
	//switch the positions in the heap
	heap[i] = t2;
	heap[j] = t;

}

//restore order from the bottom up
void heapfixUp(int k)
{
	
	while (k>1 && (heap[k/2].freq < heap[k].freq)) //while there's more than 2 items in queue & the item is less than its child
	{
		heapexch(k, (k/2)); //send the values to exchange
		k = (k/2); //examine the parent next
	}
}

//restore order from the top down
void heapfixDown(int k, int N)
  { int j, i, v;

    while ((2*k) <= N)
      { j = (2*k);
      
        if (j < N && (heap[j].freq < heap[j+1].freq)) 
			{
				j++;
			}
		//if the heap condition is met, do nothing
        if (heap[k].freq > heap[j].freq) 
        {
			break;
		}
		//if the condition is not met, exchange the position of the items
        heapexch(k, j);
        k = j;
      }
  }



void heapinit()
{	
	R = 0;
}

int heapempty(){
	//if there are no items in the queue, return 0
	return R == 0;
}

void heapinsert(Item k)
{
	int i;
	R++;
	//heap array at newly increased index is the new item
	heap[R] = k;
	
	//restore heap condition with new number of items
	heapfixUp(R);
}

//delete the largest item in the heap and return it
Item heapdelmax()
{
	
	int i;
	
	//exchange the top and bottom items of the heap
	heapexch(1, R);
	
	//restore heap condition on the items excluding the largest
	heapfixDown(1, --R);

	//return the largest item, which is now outside the range of the heaparray
	return heap[R+1];

}


//return the lowest item w/out deleting it
Item heapmin()
{
	//printf("returning %i\n", heap[R].freq);
	return heap[R];
}

//return the max item w/out deleting it
Item heapmax()
{
		//printf("returning %i\n", heap[1].freq);

	return heap[1];
}




