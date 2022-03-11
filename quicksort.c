#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Controllers */
#define less(a, b) ((a) < (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

/* Operators */
#define swap(a, b) do { int tmp = a; a = b; b = tmp; } while(0)
#define rotate(a, b, c) do { int tmp = a; a = b; b = c; c = tmp; } while(0)

/* Functions */
static void insert_sort(int *left, int *right);
static void sort(int *arr, int len);
static void qusort(int* left, int* right);
static int* partition(int *left, int *right);
static int parse();


/* Main Program */
int main() {
  int size = parse();
  int array[size];
  int *ptr = array;
  int c_val = size, c_char;
  do {
    c_char = parse();
    *ptr++ = c_char;    
  } while(--c_val > 0);
  
  sort(array, size);

  for(int i = 0; i < size; i++)
    printf("%d ", array[i]);
  return 0;
}   

/* Implementations */

/* Insertion sort using pointer and reference instead of array and value */
static 
void insert_sort(int *left, int *right) {
  /* Get the smallest value and swap to the first index */
  for (int *pindex = left + 1; pindex <= right; pindex++)
    /* If value is smaller than left most value, swap places */
    if (less(*pindex, *left))
      swap(*pindex, *left);

  /* Move value */
  for (int *pindex = left + 2; pindex <= right; pindex++) {
    int h = *pindex;
    int *prev = pindex - 1;
    /* If current value is smaller than previous elements, move them to the right */
    while (less(h, *prev)) {
      *(prev + 1) = *prev;
      prev -= 1;
    }
    /* Place element when no smaller elements are before the current element */
    *(prev + 1) = h;
  }
}

/* Hoare's fast parition (iterative) */
/* https://en.wikipedia.org/wiki/Quicksort#Hoare_partition_scheme */
static 
int* partition(int* left, int* right) {

    /* Get the left most index */
    int *left0 = left;
    int pivl = *left;
    left += 1;

    /* Get the right most index */
    int pivr = *right;

    /* Get middle value */
    int* mid = left0 + ((right - left0) >> 1);    
    int piv = *mid;

    /* Median-of-three algorthm*/
    if(less(piv, pivl)) { 
      if(less(pivr, pivl)) {   
        if(less(pivr, piv)) 
          swap(pivl, pivr);        
        else rotate(pivl, piv, pivr);          
      } 
      else swap(pivl, piv); 
    } else { 
      if(less(pivr, piv))              
        if(less(pivr, pivl)) 
          rotate(pivr, piv, pivl);  
        else swap(piv, pivr);                 
    }  

    /* Update and set pivots */
    *right = pivr;
    *mid = *left;
    *left0 = pivl;
    *left = piv;
    
    while (1) {
      /* Move the right and left pivot towards the middle */
      do left += 1; while (less(*left, piv));
      do right -= 1; while (less(piv, *right));

      /* When left is larger than right, break */
      if (left >= right) break;

      /* If not swap value and repeat babyyyyyy */
      swap(*left, *right);
    }

    *(left0 + 1) = *right;
    *right = piv;
    return right;
}

static 
void sort(int *data, int len) {
  /* Make own stack */
  int *stack[64];
  /* Make stack pointer a register variable as it will be used often */
  register int sp = 0;

  /* Left takes same address as the first element in data */
  int *left = data;
  /* Right takes same address as the last element in data */
  int *right = data + len - 1;

  while (1) {
    /* If size of partition or array is below 32 elements, do insertion sort instead */
    if (32 > right - left) {
      insert_sort(left, right);
    
      /* If stack is empty, stop dawg */
      if (sp == 0) break;

      /* If not, continue partitioniong */
      sp -= 2;
      left = stack[sp];
      right = stack[sp + 1];
    
    } else {
      /* Parition the array using hoar's parition */
      int *mid = partition(left, right);
      /* If mid is smaller than median, swap right with mid on stack */
      if (mid < left + ((right - left) >> 1)) {
        stack[sp] = mid + 1;
        stack[sp + 1] = right;
        right = mid - 1;
      } else {
        stack[sp] = left;
        stack[sp + 1] = mid - 1;
        left = mid + 1;
      }
      /* Move up in stack */
      sp += 2;
    }
  }
}

static 
int parse() {
  register int c, n, m = 0;
  /* Get character from stdin stream with no formatting */
  n = getchar_unlocked();
  
  /* If characters is end of file, return EOF */
  if(n == EOF) return n;

  /* If characters is '-', make boolean value m to one */
  if(n == '-') { m = 1; n = 0; }
  /* If not, subtract character value of '0' from character value */
  /* This works because 0123456789 are '0' + 1,2,3,4,5,6,7,8,9 */
  else n -= '0';

  /* Read characters until a character has a lower value than 0 is given */
  while((c = getchar_unlocked()) >= '0')
    /* Mutiply current value by 10 and add new character */
    n = ((n << 3) + (n << 1)) + c - '0';

  /* Return value as negative if number is supposed to be negative, else return positive number */
  return (m) ? -n : n;
}