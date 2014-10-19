/*
 * Bill McMillin
 * FCM 709 
 * Project 2
 * Professor Johnson
 * 5/21/2013
 * 
 */ 

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "Item.h"
#include "pq.c"
#include "heap.c"

//string to hold input text
char inString[99999] = {0};

//length of input file
int length = 0;

//maximum number of chars to be encoded
#define maxN 64

//make each char an item so it can be associated with a frequency
static Item allChars[64];

//this will hold only the characters that appear in the file
static Item toEncrypt[64];

//like toEncrypt, but holds the chars in order
static Item toTree[64];

//holds the position in toTree of the character in [0][i]
//and the frequency in [1][i]
int codeArray[2][64] = {NULL};

//holds the parent item of each item in the tree
int dad[maxN * 3] = {0};

//holds all items in the tree
Item treeArray[maxN * maxN] = {NULL};

//length of binary code
int len[maxN] = {NULL};

//int of binary code
int code [maxN] = {NULL};

//used to test pq
int top = 0;

//used to test pq
int qlength  = 0;
int y;

//initial treelength and expanded treelength to track length as it's built
int treelength, exptreelen;
    
    
//get the frequencies of characters    
void populate_freq()
{
	//get the input file and put it in a string
    FILE* fp =  fopen("text.txt", "r");
    if(fp==NULL) 
		{puts("problem with input file");exit(-1);}
    char c;
    int i = 0;
    int j, count;
    while((c=getc(fp)) != EOF)
         
       {
        inString[i] = c;

        length++;
        i++;
           
       }
       


	//store each character in an item in an array of items

	for(i=0; i<62; i++)
	{
		allChars[i].chrctr = 'A' + i;
	}

	//add the space character
	allChars[63].chrctr = 32;

	
    //calculate the frequency of each character, including space and store
    //it in the item

	
	for(j=0;j<64;j++)
	{
		count = 0;

		for(i=0; i<length; i++)
		{
			if(inString[i] == allChars[j].chrctr)
			{
				count ++;
			}
		}
		allChars[j].freq = count;
		//printf("stored freq = %i\n", allChars[j].freq);
	}

}

//used to put the char Items in a priority queue and insert them
//into the heap that will be used to build a tree
/*
 * pq and heap are basically the same thing, but pq returns 
 * the lowest values and heap returns the highest 
 */
  
void queuePrint()
{
	int i, j, q;
	Item top, temp1, temp2;
	PQinit();
	j=0;
	for(i=1;i<=64;i++)
	{
		//if the character appears
		if(allChars[i].freq > 0)
		{
			//put it in the array to encrypt
			toEncrypt[j] = allChars[i];
			j++;
		}
	}
	for(i=1;i<=64; i++)
	{
		//make sure only items with a frequency get put in queue
		if(toEncrypt[i].freq > 0)
		{
			//insert the items into the pq
			PQinsert(toEncrypt[i]);
		}	
	}
	q = 0;
	treelength = 0;
	
	while(!PQempty())
	{
	//create a temp item by deleting the smallest item from the pq
	temp1 = PQdelmin();
	//put it in the array that will be used to build the tree
	toTree[q] = temp1;
	//make sure to associate the array position of the item with the item
	toTree[q].pos = q;

	treelength++;
	q++;
	
	}
	
} 

//function from Sedgewick to turn an int into bits
unsigned bits(unsigned x, int k, int j)
{
	return (x >> k) & ~(~0 << j);
}

//takes array of char items and builds the tree
void makeTree()
{
	Item tempa, tempb, tempc;
	int i, j, k, x, q, u;

		
	heapinit();
	exptreelen = treelength;
	
	//this part builds the bottom two layers of the tree, the rest are built later
	
	//increase by 2 since we're using two items at a time and combining them to 1
	for(i=0,j=1; i<treelength; i+=2, j++)
	{
		//create a temp item, get the two lowest frequencies and add them
		//store the new key in the temp item 
		tempa.freq = (toTree[i].freq + toTree[i+1].freq);
		//the temp item will go in the tree at the end of the current tree array
		tempa.pos = (treelength+j);
		//give the temp item a l and r child, using their array position as the key
		tempa.l = toTree[i].pos;
		tempa.r = toTree[i+1].pos;
		tempa.chrctr = NULL; //to distinguish the new item from leaf items
		treeArray[treelength+j] = tempa; //now put it in the array
		treeArray[treelength+j].freq = tempa.freq; //should be unnecessary, just testing
		treeArray[treelength+j].l = tempa.pos; //ditto
		treeArray[treelength+j].r = tempb.pos; //ditto
		treeArray[toTree[i].pos] = toTree[i]; //add the leaf nodes to the treeArray
		treeArray[toTree[i+1].pos] = toTree[i+1];
		
		dad[toTree[i].pos] = tempa.pos; //use the dad array to keep track of the parent relationship
		dad[toTree[i+1].pos] = -tempa.pos; //make the right child negative to distinguish it from left
	
		heapinsert(toTree[i]); //put the leaf into the heap
		heapinsert(toTree[i+1]); //put the second leaf into the heap
		heapinsert(treeArray[treelength+j]); //put the parent item into the tree
		PQinsert(tempa); //insert only the parent item into the pq
		exptreelen++; //expand the tree length 
	}
	
	//this part builds the rest of the tree using only non-leaf nodes from the pq
	//continue until there is only one node at the top of the tree
	while(!PQempty())
	{
		//get the 2 lowest items
		tempa = PQdelmin();
		tempb = PQdelmin();
		//combine their frequency and store it in a parent node
		tempc.freq = (tempa.freq + tempb.freq);
		tempc.chrctr = NULL; //to distinguish it from a leaf
		tempc.l = tempa.pos; //left child is the lowest item
		tempc.r = tempb.pos; //right child is the second lowest item
		tempc.pos = (++exptreelen); //associate the array position with the parent item
		treeArray[exptreelen] = tempc; //put the parent node in the treeArray
		dad[tempa.pos] = tempc.pos; //left child
		dad[tempb.pos] = -tempc.pos; //right child negative
		
		heapinsert(tempc); //put only the parent into the pq
		PQinsert(tempc);
		
	}
	
	//from Sedgewick
	for(k=0; k<treelength; k++)
	{
		if (!treeArray[k].freq)
		{
			code[k] = 0;
			len[k] = 0;
		}
		else
		{
			i=0; 
			j=1;
			u = dad[k];
			x = 0;
			
			while(u)
			{
				if(u < 0)
				{
					x += j;
					u = -u;
					//printf("loopx %i\n", x);
				}
				u = dad[u];
				j += j;
				i++;
			}
			code[k] = x;
			len[k] = i;
			//printf("postloopx %i %i code:%i\n", x, k, code[k]);

		}
		codeArray[0][k] = k; //store the array position in the codeArray table
		codeArray[1][k] = x; //store the code in the codeArray table
		//printf("postloop2 %i %i code:%i\n", x, k, code[k]);

		
		
	}
	

//print the code table
	for (i = 0; i < treelength; i++)
		{
			printf("table char:%c code:", treeArray[codeArray[0][i]].chrctr);

			for (j = len[i]; j > 0; j--)
			{
				printf("%1d", bits(code[i], j-1,1));
			}
			printf("\n");
		} 

}
	
void parsefile()
{
	//get the input file and put it in a string
    FILE* fp =  fopen("text.txt", "r");
    if(fp==NULL) 
		{puts("problem with input file");exit(-1);}
	FILE * wfp;
	
	//open the output file for writing
	wfp = fopen ("encoded.txt", "w+");
    char c;
    int i = 0;
    int j, k, count;
    while((c=getc(fp)) != EOF)
         
       {
        inString[i] = c;

        length++;
        i++;
       //compare each character to the treeArray    
       for(j=0; j<treelength; j++)
		{
			if(c == treeArray[codeArray[0][j]].chrctr)
			{
				//when a match is found
				for (k = len[j]; k > 0; k--)
				{
					//output the value of that letter's code to the file in binary
					fprintf(wfp, "%1d", bits(code[j], k-1,1));

				}
			}
		}
     }
     fclose(fp);
     fclose(wfp);
}

int main()
{   
	//get the frequencies of the input file and put them in a table
    populate_freq();
	
	//put the frequencies in a queue and build the precursor to the tree
    queuePrint();
    
    //use the heap to build a tree with the letters as leaf nodes
    makeTree();
    
    //read the output file and output the binary code into an output file
    parsefile();
  }
   
   
   
