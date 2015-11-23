/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int main(void)
{
    
    // open input file, card.raw
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open the file");
        return 1;
    }

    // find file's size
    struct stat st;
    stat("card.raw", &st);
    int filesize = st.st_size;
    
    // allocate memory to read the whole file
    unsigned char *jpgread;
    jpgread = malloc(filesize);
    
    // variables to track how many image files are found, and where an image starts. Initialize i early to deal with an image outside of the loop
    int i;
    int filecount = 0;
    int imagebeg = 0;
              
    // read a byte until the file is over, but don't read past the memory allocated for the file
    for (i = 0; (fread(jpgread, sizeof(unsigned char), 1, inptr) != 0); i++)
    {   
        
    }
    printf("i is %i", i);
    

            
                    
    // close outfile
    fclose(outptr);
    fclose(inptr);
                                               
    // free the memory for the array   
    free (jpgread);
               
}
