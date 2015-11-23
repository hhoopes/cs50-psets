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

    // remember the resize factor
    int factor = atoi(argv[1]);
    
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
    printf ("%x/n, bf.bfType);
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
    bf_new.bfSize = bf.bfSize * factor;
    bi_new.biSizeImage = (factor * bi.biSize) - 54;
    bi_new.biHeight = bi.biHeight * factor;
    bi_new.biWidth = bi.biWidth * factor;
   
    // write outfile's BITMAPFILEHEADER piece by piece in order
    fwrite(&bf.bfType, sizeof(bf.bfType), 1, outptr);
    fwrite(&bf_new.bfSize, sizeof(bf_new.bfSize), 1, outptr);
    fwrite(&bf.bfReserved1, sizeof(bf.bfReserved1), 1, outptr);
    fwrite(&bf.bfReserved2, sizeof(bf.bfReserved2), 1, outptr);
    fwrite(&bf.bfOffBits, sizeof(bf.bfOffBits), 1, outptr);


    // write outfile's BITMAPINFOHEADER, piece by piece in order
    fwrite(&bi_new.biSize, sizeof(bi_new.biSize), 1, outptr);
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
    

    // determine padding for scanlines
    int padding =  (4 - (bi_new.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi_new.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi_new.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            
            for (int k = 0; k < factor; k++)

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
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
    
    return 0;
}

