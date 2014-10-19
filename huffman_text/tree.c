#include <stdlib.h>
#include "Treeindex.h"
#define maxTree 64

static int R;

static int treeN;

//create an array of Items 
static Item tree[maxTree+1];


//exchange Items in the tree
void exchTree(int i, int j)
{

	Item t,t2; //create 2 temporary Items 
	t = tree[i];  
	t2 = tree[j];
	
	tree[i] = t2;
	tree[j] = t;

}


void fixUpTree(int k)
{
	int i;
	while (k>1 && (tree[k/2].freq > tree[k].freq)) //while there's more than 2 items in queue & the item is less than its child
	{
		exchTree(k, (k/2)); //send the values to exchange
		k = (k/2); //examine the parent next
	}
	
	for(i=0; i<=R+1; i++)
		{
		//	printf("treearray %i: %i\n", i, tree[i].freq);
		}
}

void fixDownTree(int k, int R)
  { int j, i, v;

    while ((2*k) <= R)
      { j = (2*k);
      
        if (j < R && (tree[j].freq < tree[j+1].freq)) 
			{
			//	printf("in this loop %i times\n", j);
				j++;
			}
        if (tree[k].freq > tree[j].freq) 
        {
			//printf("breaking at %i: %i\n", k, j);

			break;
		}
		for(i=0; i<=R+1; i++)
		{
		//	printf("treearray b4exch %i: %i\n", i, tree[i]);
		}
		//printf("fxdn exchg %i: %i\n", tree[k].freq, tree[j].freq);

        exchTree(k, j);
        k = j;
      }
  }




void Tree_init()
{	
	R = 0;
	//printf("initializing tree\n");
}

int Tree_empty(){
	return R == 0;
}

void Tree_insert(Item k)
{
	//printf("inserting to tree: %i\n", k.freq);
	int i;
	R++;

	tree[R] = k;
	
	
	fixUpTree(R);
	for(i=0; i<=R+1; i++)
		{
			//printf("insert treearray at %i: %i n:%i\n", i, tree[i],N);
		}
	//printf("now in tree: %i\n", tree[N].freq);
	//printf("top of tree %i\n", tree[1].freq);
	
}

Item Treedelmax()
{
	
	int i;
	for(i=0; i<=R+1; i++)
		{
		//	printf("b4fd treearray at %i: %i n:%i\n", i, tree[i].freq,R);
		}
	
	for(i=0; i<=R+1; i++)
		{
		//	printf("b4fxdn treearray at %i: %i n:%i\n", i, tree[i].freq,R);
		}
	
	exchTree(1, R);
	
	fixDownTree(1, --R);

for(i=0; i<=R+1; i++)
		{
		//	printf("aftrfx treearray at %i: %i n:%i\n", i, tree[i].freq,R);
		}
	
	//printf("returning %i\n", tree[R+1].freq);
	
	return tree[1];
	

}

char Treeminchar()
{
	printf("returning %i\n", tree[R].chrctr);
}

char Treemaxchar()
{
		printf("returning %c\n", tree[1].chrctr);

}




