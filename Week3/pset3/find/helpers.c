/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // implement a searching algorithm
    int left = 0;
    int right = n-1;
    int middle = 0;
    while (n > 0)
        {
        middle = (right+left)/2;
        
        //if the value is in the middle
        if (value == values[middle]) 
            return true;
        
        //if the value is greater than the middle value
        else if (value > values[middle])
            {
            left = middle +1;
            n = n/2;
            
            }
            
        //if the value is less than the middle value
       else if (value < values[middle])   
            {
            right = middle - 1;
            n = n/2;          
            }
       
        }
     return false;
        
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // implement an O(n^2) sorting algorithm (bubble sort)
    
    // loop to compare first two values in array
    int needswap = 1;
    while (needswap > 0)
        {
        for (int i = 0; i < n-1; i++)
            {  
            needswap = 0;
            int swap = 0;
            if (values[i] >  values[i+1])
                {
                swap = values[i];
                values[i] = values[i+1];
                values[i+1]= swap;
                needswap = needswap +1;
                }
         
            }
        }
    return;
}
