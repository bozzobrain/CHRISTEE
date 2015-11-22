#include <xc.h>
#include "sort.h"

void insertionSort( unsigned int *num, int numLength)
{
     int i, j, key;
     numLength = numLength/2;
     for(j = 1; j < numLength; j++)    // Start with 1 (not 0)
    {
           key = num[j];
           for(i = j - 1; (i >= 0) && (num[i] < key); i--)   // Smaller values move up
          {
                 num[i+1] = num[i];
          }
         num[i+1] = key;    //Put key into its proper location
     }
     return;
}
