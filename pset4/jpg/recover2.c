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
#include <stdbool.h>


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
         
    // variables to track how many image files are found. Initialize outptr early
    int filecount = 0;
    FILE *outptr;
    
    for (int i = 0; i + 3 < filesize; i++)
    {                  
        // name a new file and open it if the last loop found a new file 
        unsigned char *jpgread;
        jpgread = malloc(sizeof(unsigned char)*4);
        
        fread(jpgread, sizeof(unsigned char), 4, inptr);

        if ((jpgread[0] == 0xff) && (jpgread[1] == 0xd8) && (jpgread[2] == 0xff) && ((jpgread[3] == 0xe0) || (jpgread[3] == 0xe1)))
        {          
            // close the last file
            if (filecount > 0)
            {
                fclose(outptr);                   
            }
                
            // set variables for a new file             
            filecount++;
             
            char filename[sizeof "999.jpg"];
            sprintf(filename, "%03d.jpg", filecount);                       
            outptr = fopen(filename, "w");
                
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Couldn't create %s.\n", filename);
                return 2;
            }
                                              
            // write the new bytes               
            fwrite(&jpgread, sizeof(unsigned char), 4, outptr);
                                 
            free (jpgread);                           
        }     
                         
        // if not a header (i.e., middle of a file)
        else        
        {
            fwrite(&jpgread, sizeof(unsigned char), 4, outptr);
            free (jpgread);             
        }
    }        
}
