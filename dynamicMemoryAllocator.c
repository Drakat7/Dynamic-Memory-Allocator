#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Block 
{
  int block_size; // # of bytes in the data section
  struct Block *next_block; // pointer to the next block
};

const int block_overhead_size = sizeof(struct Block); // size of a struct Block
const int void_size = sizeof(void*); // size of a void pointer

struct Block *free_head; // free head (always points to first element of free list)

// The data section follows in memory after the above struct
void my_initialize_heap(int size) 
{
  struct Block* buffer = (struct Block *) malloc(size); // initial block
  free_head = buffer; // point free head to buffer
  free_head->block_size = size - block_overhead_size; // set block size of free head
  free_head->next_block = NULL; // set next block of free head
  printf("voidsize: %d, blockoverhead: %d\n", void_size, block_overhead_size);
}

void* my_malloc(int size)
{
  struct Block* previous; // previous block pointer
  struct Block* current = free_head; // current block pointer, always starts at free head
  //setting size to be a multiple of void size for fragmentation purposes
  //Matthew was here
  // if(size % void_size != 0)
  // {
  //   size += void_size - (size % void_size); 
  // }
  
  //William was here
  if(size % void_size != 0)
  {
    size = void_size * ((size / void_size) + 1);
  }
  //look for next available free block
  while(current != NULL)
  { 
    //check if current block is of large enough size
    if(current->block_size >= size)
    {
      //check if current block can be split, if it can, do it
      //otherwise remove the current block from the free list
      if((current->block_size - size) >= (block_overhead_size + void_size))
      {
        //p becomes the new block
        struct Block* p = (struct Block*)((char*)current + block_overhead_size + size); //  block_overhead_size;
        p->block_size = current->block_size - (size + block_overhead_size); //allocating new blocks size
        current->block_size = size; // adjusting current block size
        // set appropriate pointers depending on current
        p->next_block = current->next_block; //
        if(current == free_head)
        {
          free_head = p;
        }
        else
        {
          previous->next_block = p; //point previous to p
        }
      }else 
      {
        // remove blocks depending on current
        if(current == free_head)
        {
          free_head = free_head->next_block;
        }else
        {
          previous->next_block = current->next_block;
        }
      }
      //return a pointer to the data region of the allocated block
      return (char*)current + block_overhead_size;
    }else
    // traverse free list
    {
      previous = current;
      current = previous->next_block;
    }
  }
  // no appropriate block found
  return 0;
}

// p is the address block we want to free
void my_free(void * data)
{
  struct Block* q = (struct Block *)data - 1; //provided by professor
  q -> next_block = free_head; //q points to the old address of free_head
  free_head = q; //q becomes the new free head
}

float standard_deviation_test()
{
  //doubles are rounded down
  char *string;
  int n = 0;
  while(n <= 0)
  {
    printf("Input positive integer for array size: ");
    scanf("%s", string);
    n = atoi(string);
  }
  int* base = (int*)my_malloc(sizeof(int[n]));
  int total = 0;
  //filling array
  for(int j=0; j<n; j++)
  {
    char *junk;
    int input;
    printf("Input %dth value: ", j);
    int read = scanf("%d", &input);
    while(read != 1)
    {
      scanf("%s", junk);
      printf("Input %dth value: ", j);
      read = scanf("%d", &input);
    }
    base[j] = input;
    total += input;
  }
  float mean = (float)total / n;
  
  float sum = 0;
  
  float sDeviation = 0;
  //std deviation calculation
  for (int i = 0; i < n; i++)
  {
    float temp_sum = (float)base[i] - mean;
    sum += temp_sum * temp_sum; //square them
  }
  return sqrt(sum/n); 
}


int main(void) 
{  
  my_initialize_heap(1000000000);

  /* TESTING */
  /*  Test 1 */
  printf("\nTest 1: Allocate(int), Free(int), Allocate(int)\n");
  int *x = my_malloc(sizeof(int));
  printf("Address in x is %p. \n", x);
  my_free(x);
  //x = NULL;
  int *y = my_malloc(sizeof(int));
  printf("Address in y is %p. \n", y);

  //Test 2
  printf("\nTest 2: Allocate(int), Allocate(int)\n");
  int *w = my_malloc(sizeof(int));
  printf("Address in w is %p. \n", w);
  int *z = my_malloc(sizeof(int));
  printf("Address in z is %p. \n", z);

  //Test 3
  printf("\nTest 3: Allocate(int), Allocate(int), Allocate(int), Free(int), Allocate(double)\n");
  int *a = my_malloc(sizeof(int));
  int *b = my_malloc(sizeof(int));
  int *c = my_malloc(sizeof(int));
  printf("Address in a is %p. \n", a);
  printf("Address in b is %p. \n", b);
  printf("Address in c is %p. \n", c);
  my_free(b);
  double *d = my_malloc(sizeof(double));
  printf("Address in d is %p. \n", d);
  
  //Test 4
  printf("\nTest 4: Allocate(char), Allocate(int)\n");
  char *e = my_malloc(sizeof(char));
  int *f = my_malloc(sizeof(int));
  printf("Address in e is %p. \n", e);
  printf("Address in f is %p. \n", f);
  
  //Test 5
  printf("\nTest 5: Allocate(int[]), Allocate(int), Free(int[])\n");
  int *g = my_malloc(sizeof(int[100]));
  int *h = my_malloc(sizeof(int));
  printf("Address in g is %p. \n", g);
  printf("Address in h is %p. \n", h);
  my_free(g);
  printf("Address in h is %p. \n", h);
  printf("Value of h is %d. \n", *h);

  //Standard deviation test
  printf("\nTest 6: Standard Deviation\n");
  printf("Standard Deviation: %f\n", standard_deviation_test());
  return 0;

}




