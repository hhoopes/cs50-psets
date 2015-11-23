/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
       
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIMIT 65536

int main(int argc, string argv[])
{
    // Offers usage rule if the user doesn't provide either 1 or 2 command line arguments other than the name of the program
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // converts the string in the 2nd command line argument into an integer value and initializes n as that value
    int n = atoi(argv[1]);

    // If the user provides a seed value in the 3 command line argument, use that to seed the srand function. Else seed the function with the time
    if (argc == 3)
    {
        srand((unsigned int) atoi(argv[2]));
    }
    else
    {
        srand((unsigned int) time(NULL));
    }

    // ls
    print out the list of all the random numbers for as many values as the user specified, with the generated random numbers being modulo'ed by the limit defined above, so they don't get bigger than it
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", rand() % LIMIT);
    }

    // that's all folks
    return 0;
}
