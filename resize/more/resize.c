// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>


#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // declaring n
    float f = atoi(argv[1]);


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


    // OLD VALUES HERE
    int oldwidth = bi.biWidth;
    int oldheight = abs(bi.biHeight);
    int oldpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // NEW VALUES HERE
    bi.biWidth *= f;
    bi.biHeight *= f;
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4; // determine total padding required per scanline
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // LET'S FINISH THE JOB....!!!!!!!!!!!!!



    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < oldheight; i++)
    {
        // resize vertically
        for (int a = 0; a < f; a++)
        {


            // iterate over pixels in scanline
            for (int j = 0; j < oldwidth; j++)
            {

                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // resize horizontally
                for (int k = 0; k < f; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

            // skip over padding, if any
            fseek(inptr, oldpadding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }


            // move back to start of current row
            fseek(inptr, -((oldwidth * sizeof(RGBTRIPLE)) + oldpadding), SEEK_CUR);

        }


        // move pointer to end of scanline - so we can get the next row
        fseek(inptr, (oldwidth * sizeof(RGBTRIPLE)) + oldpadding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
