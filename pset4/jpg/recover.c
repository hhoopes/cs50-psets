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
    
    fread(jpgread, sizeof(unsigned char), filesize, inptr);
      
    // read a byte until the file is over, but don't read past the memory allocated for the file
    for (i = 0; (i + 3) < filesize; i++)
    {   
        // math to store the size of an image based on when it starts                   
        int imagesize = i - imagebeg;
                  
        // search for the bytes that make a jpg header              
        if ((jpgread[i] == 0xff) && (jpgread[i + 1] == 0xd8) && (jpgread[i + 2] == 0xff) && ((jpgread[i + 3] == 0xe0) || (jpgread[i + 3] == 0xe1)))
        {
            // first time find a header, set the imagebeg and increment filecount
            if (filecount == 0)
            {
                imagebeg = i;
                filecount++;                                
            }
                
            // after you find the first image
            else
            {   
                // name the file and open it                   
                char filename[sizeof "999.jpg"];
                sprintf(filename, "%03d.jpg", filecount - 1);        
                
                FILE* outptr = fopen(filename, "w");
                if (outptr == NULL)
                {
                    fclose(inptr);
                    fprintf(stderr, "Could not create %s.\n", filename);
                    return 2;
                }
                    
                // write the file, increment the number of files, and save the starting spot for the next image
                fwrite(&jpgread[imagebeg], sizeof(unsigned char), imagesize, outptr);
                  
                // close outfile
                fclose(outptr);
                    
                filecount++;
                imagebeg = i;                                   
            }
        } 
    }
           
    // last image that won't be found by the for loop
    char filename[sizeof "999.jpg"];
    sprintf(filename, "%03d.jpg", filecount - 1);        
                
    FILE* outptr = fopen(filename, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", filename);
        return 2;
    }
            
    fwrite(&jpgread[imagebeg], sizeof(unsigned char), filesize - imagebeg, outptr);
                    
    // close outfile
    fclose(outptr);
    fclose(inptr);
                                               
    // free the memory for the array   
    free (jpgread);
               
}
