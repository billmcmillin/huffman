#include <stdlib.h>
#include "PQindex.h"
#define maxPQ 200


//to hold the number of items in the queue
static int N;

//create an array of Items 
static Item pq[maxPQ+1];


//exchange Items in the pq
void exch(int i, int j)
{

	Item t,t2; //create 2 temporary Items 
	t = pq[i];  
	t2 = pq[j];
	
	//exchange the position of the items
	pq[i] = t2;
	pq[j] = t;

}

//restore the heap condition from the bottom up
void fixUp(int k)
{
	
	while (k>1 && (pq[k/2].freq > pq[k].freq)) //while there's more than 2 items in queue & the item is less than its child
	{
		exch(k, (k/2)); //send the values to exchange
		k = (k/2); //examine the parent next
	}
}

//restore the heap condition from the top down
void fixDown(int k, int N)
  { int j, i, v;

    while ((2*k) <= N)
      { j = (2*k);
      
        if (j < N && (pq[j].freq > pq[j+1].freq)) 
			{
				j++;
			}
        if (pq[k].freq < pq[j].freq) 
        {
			break;
		}

        exch(k, j);
        k = j;
      }
  }


void PQinit()
{	
	N = 0;
}

//if no items are the pq, return 0
int PQempty(){
	return N == 0;
}

void PQinsert(Item k)
{
	int i;
	N++;
	//increase the size of the pq

	//put the new item at the end of the array
	pq[N] = k;
	
	//restore the heap condition w/ the new number of items in the array
	fixUp(N);
}

//delete the item w/ the smallest key(freq) and return it
Item PQdelmin()
{
	
	int i;
	//exchange the top and bottom items
	exch(1, N);
	
	//restore the heap condition on the remaining items, reducing the size of the pq
	fixDown(1, --N);

	//return the item that is now outside the range of the pq
	return pq[N+1];

}

//return the min item w/out deleting it
Item PQmin()
{
	return pq[N+1];
}

//return the max item w/out deleting it

Item PQmax()
{
		
	return pq[1];
}




