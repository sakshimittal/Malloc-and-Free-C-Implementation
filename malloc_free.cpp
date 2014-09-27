#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef unsigned char Byte;
Byte *pool; 
int poolSize = 0;

typedef struct List
{
	Byte *front;
	Byte *rear;
	struct List *next;
}FreeList, *FreeListptr;

FreeListptr start = NULL;

Byte * checkBytePointer(int size);
int addBytePointer(Byte *byteFront, Byte *byteRear);
int deleteBytePointer(Byte *byteFront, Byte *byteRear);
Byte * findRearPointer(Byte *byteFront);

int initPool();
void terminatePool();
void *mallocFromPool(int size);
void freeToPool(void *);

void main()
{
	int allocated;
	allocated=initPool();
	//printf("%d",poolSize);
	if(allocated==0)
	{
		printf("\nError. Could not allocate pool.");
	}

	int *b=(int*)mallocFromPool(4*(sizeof(int)));
	printf("\nEnter an array of integers : \n");
	for(int i=0 ; i<4 ; i++)
	{
		scanf("%d",&b[i]);
	}

	int *d=(int*)mallocFromPool(8*(sizeof(int)));
	printf("\nEnter an array of integers : \n");
	for(int i=0 ; i<8 ; i++)
	{
		scanf("%d",&d[i]);
	}

	freeToPool(b);

	int *c=(int*)mallocFromPool(8*(sizeof(int)));
	printf("\nEnter an array of integers : \n");
	for(int i=0 ; i<8 ; i++)
	{
		scanf("%d",&c[i]);
	}
	

	printf("\n\n");
	for(int j=0 ; j<poolSize ; j++)
	{
		printf("%d",pool[j]);
	}
	terminatePool();
}

int initPool()
{
	printf("\nEnter the size of pool : ");
	scanf("%d",&poolSize);

	pool = (Byte*)malloc(poolSize*sizeof(Byte));
	if(pool==NULL)
	{
		return 0;
	}
	else
	{
		memset(pool,'\0',poolSize);
		return 1;
	}
}

void terminatePool()
{
	free(pool);
}

void *mallocFromPool(int size)
{
	Byte *frontPointer = checkBytePointer(size);
	if(frontPointer == pool)
	{
		Byte *rearPointer = pool+size-1;
		if(rearPointer > (pool+poolSize-1))
		{
			return NULL;
		}
		addBytePointer(pool,rearPointer);
		return (void *)pool;
	}
	else
	{
		if(frontPointer > (pool+poolSize-1))
		{
			return NULL;
		}

		Byte *rearPointer = frontPointer+size-1;
		if(rearPointer > (pool+poolSize-1))
		{
			return NULL;
		}
		
		addBytePointer(frontPointer,rearPointer);
		return (void *)frontPointer;
	}
}

void freeToPool(void *memoryLocationName)
{
	Byte *rearPointer = findRearPointer((Byte *)memoryLocationName);
	int success = deleteBytePointer((Byte *)memoryLocationName,rearPointer);
	if(success == 0)
	{
		printf("\nError occured.Could not deallocate memory.");
	}
	else
	{
		Byte *frontPointer = (Byte *)memoryLocationName;
		for(int i=0 ; i<rearPointer - frontPointer + 1 ; i++)
		{
			*(frontPointer + i) = '\0';
		}
	}
}

Byte * checkBytePointer(int size)
{
	if(start==NULL)
	{
		return pool;
	}

	FreeListptr current = start;
	FreeListptr previous = current;
	
	if(current->front != pool)
	{
		if(((current->front) - pool)>=size)
		{	
			return pool;
		}
	}

	current = current->next;
	while(current != NULL)
	{
		if(((current->front) - (previous->rear) - 1) >= size)
		{	
			return (previous->rear)+1;
		}
		previous = current;
		current = current->next;
	}
	return (previous->rear)+1;
}

int addBytePointer(Byte *byteFront, Byte *byteRear)
{
	FreeListptr current = start;
	FreeListptr previous = current;
	FreeListptr newnode;

	newnode = (FreeListptr)malloc(sizeof(FreeList));
	newnode->front=byteFront;
	newnode->rear=byteRear;
	newnode->next=NULL;

	if(start==NULL)
	{
		start=newnode;
		return 1;
	}
	else if(current->front > byteFront)
	{
		if(current->rear == byteRear)
		{
			return 0;
		}
		newnode->next = current;
		start = newnode;
		return 1;
	}
	else if(current->front < byteFront)
	{
		while(current->next!= NULL)
		{
			if(current->front == byteFront || current->rear == byteRear)
			{
				return 0;
			}
			if(current->front < byteFront)
			{
				previous = current;
				current=current->next;
			}
			else
			{
				newnode->next = current;
				previous->next = newnode;
				return 1;
			}
		}
		
		if(previous->front == byteFront || previous->rear == byteRear)
		{
			return 0;
		}
		current->next = newnode;
		return 1;
	}
	return 0;
}

int deleteBytePointer(Byte *byteFront, Byte *byteRear)
{
	FreeListptr current = start;
	FreeListptr previous = current;
	if(start==NULL)
	{
		return 0;
	}

	if(current->front == byteFront && current->rear == byteRear)
	{
		start = current->next;
		free(current);
		return 1;
	}
	
	current=current->next;
	while(current!=NULL)
	{
		if(current->front == byteFront && current->rear == byteRear)
		{
			previous->next = current->next;
			free(current);
			return 1;
		}
		previous = current;
		current = current->next;
	}
	return 0;
}

Byte * findRearPointer(Byte *byteFront)
{
	FreeListptr current = start;
	if(start == NULL)
	{
		return NULL;
	}
	
	while(current != NULL)
	{
		if(current->front == byteFront)
		{
			return (current->rear);
		}
		current = current->next;
	}
	return NULL;
}