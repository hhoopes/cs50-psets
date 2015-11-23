/**
 * resize.c
 *
 * CS50
 * Pset 4
 * Heidi Hoopes
 * Resizes a bitmap line by line, multiplying by a user-picked factor
 */
 
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // make sure resize factor is an integer between 1 and 100
    // remember the resize factor
    int factor = atoi(argv[1]);
    if ((factor < 1) || (factor > 100))
    {
        printf("Please specify a resize factor that's a whole number between 1 and 100\n");
        return 1;
    }
    
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    
     // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
       
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // define new header constants based on the multiply factor
    BITMAPFILEHEADER bf_new;
    BITMAPINFOHEADER bi_new;
    bf_new.bfSize = bi.biSizeImage * factor + 54; 
    bi_new.biSizeImage = (factor * bi.biSizeImage);
    bi_new.biHeight = bi.biHeight * factor;
    bi_new.biWidth = bi.biWidth * factor;
   
    // write outfile's BITMAPFILEHEADER piece by piece in order
    fwrite(&bf.bfType, sizeof(bf.bfType), 1, outptr);
    fwrite(&bf_new.bfSize, sizeof(bf_new.bfSize), 1, outptr);
    fwrite(&bf.bfReserved1, sizeof(bf.bfReserved1), 1, outptr);
    fwrite(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, outptr);
    fwrite(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, outptr);


    // write outfile's BITMAPINFOHEADER, piece by piece in order
    fwrite(&bi.biSize, sizeof(bi.biSize), 1, outptr);
    fwrite(&bi_new.biWidth, sizeof(bi_new.biWidth), 1, outptr);
    fwrite(&bi_new.biHeight, sizeof(bi_new.biHeight), 1, outptr);
    fwrite(&bi.biPlanes, sizeof(bi.biPlanes), 1, outptr);
    fwrite(&bi.biBitCount, sizeof(bi.biBitCount), 1, outptr);
    fwrite(&bi.biCompression, sizeof(bi.biCompression), 1, outptr);
    fwrite(&bi_new.biSizeImage, sizeof(bi_new.biSizeImage), 1, outptr);
    fwrite(&bi.biXPelsPerMeter, sizeof(bi.biXPelsPerMeter), 1, outptr);
    fwrite(&bi.biYPelsPerMeter, sizeof(bi.biYPelsPerMeter), 1, outptr);
    fwrite(&bi.biClrUsed, sizeof(bi.biClrUsed), 1, outptr);
    fwrite(&bi.biClrImportant, sizeof(bi.biClrImportant), 1, outptr);
    

    int padding_old =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_new =  (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;      

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //assign a chunk of memory the size of a line of the unresized file
        RGBTRIPLE* rowarray = malloc((bi_new.biWidth)*sizeof(RGBTRIPLE));
        if (rowarray == NULL)
        {
            printf("Error -- out of memory.\n");
            return 1;
        }           
            
        // iterate over pixels in scanline, adding each to array
        for (int j = 0; j < bi.biWidth; j++)
        {       

            // read RGB triple from infile
                                            
            fread(&(rowarray[j]), sizeof(RGBTRIPLE), 1, inptr);            
        }    
        
          // skip over padding, if any
            fseek(inptr, padding_old, SEEK_CUR);

        
        // write a factor worth of lines
        for (int k = 0; k < factor; k++)
        {
            // iterate over all the pixels in a line
            for (int l = 0; l < bi.biWidth; l++)
            {
                // write a factor worth of pixels
                for (int m = 0; m < factor; m++)
                {    
                    // write RGB triple to outfile
                    fwrite(&rowarray[l], sizeof(RGBTRIPLE), 1, outptr);
                    
                }
            }
                                          
            // add back padding
            for (int n = 0; n < padding_new; n++)
            {   
                fputc(0x00, outptr);
            }
            
        }
        
        free(rowarray);
       
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    return 0;
}

