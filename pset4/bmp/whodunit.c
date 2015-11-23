/**
 * whodunit.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, replacing certain values of colors to view the whodunit image better
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];

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
   

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // define colored pixel data structures

    // iterate over infile's scanlines
   
        
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        
        // temporary storage
        
        RGBTRIPLE triple;
        RGBTRIPLE triple_previous;
                    
        // darken any color that isn't red
           
        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE darker_triple;
            darker_triple.rgbtBlue = triple.rgbtBlue - 0xcc;
            darker_triple.rgbtGreen = triple.rgbtGreen -0xcc;
            darker_triple.rgbtRed = triple.rgbtRed - 0xcc;
                
            // the light blues vary, red is 0000ff, the "white" is f6f6ec
    
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                      
            // If the pixel is red, change the value to the darkened pixel just before it (to attempt to undo the noise, still kludgy and weird, but want to play with more)
            if (triple.rgbtRed == 0xff)              
            {   
                // if pixel (0,0) is red, then it needs a value. needs to be fixed, perhaps by comparing other direction and using fseek    
                if ((j == 0) && (i == 0))
                {                             
                    // write RGB triple to outfile
                    fwrite(&darker_triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                
                else 
                { 
                    fwrite(&triple_previous, sizeof(RGBTRIPLE), 1, outptr);
                }
            }             
           
            //if it's not red, darken the value and then save to a variable for the next red pixel to use                
            else
            {              
                // write darker RGB triple to outfile
                fwrite(&darker_triple, sizeof(RGBTRIPLE), 1, outptr);
                triple_previous = darker_triple;
            }            
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
