#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r"); //reading the file
    int count = 0;
    uint8_t buffer[512];
    char filename[512];
    int bytenum = 1;

    while (fread(buffer, 512, 1, file))
    {
        if ((buffer[0]) == 0xff &&
            (buffer[1]) == 0xd8 &&
            (buffer[2]) == 0xff &&
            ((buffer[3]) & 0xf0) == 0xe0) // then we found a JPEG
        {
            break;
        }
    }

    while (1)
    {
        if ((buffer[0]) == 0xff &&
            (buffer[1]) == 0xd8 &&
            (buffer[2]) == 0xff &&
            ((buffer[3]) & 0xf0) == 0xe0) // then we found a JPEG

        {
            sprintf(filename, "%03i.jpg", count++);
            //1st make an array (pointer) for the file to deal
            //2nd a real filename, 3rd the number of it
        }
        FILE *img = fopen(filename, "w");
        fwrite(buffer, 512, 1, img);
        bytenum = fread(buffer, 512, 1, file);

        while (((buffer[0]) == 0xff &&
                (buffer[1]) == 0xd8 &&
                (buffer[2]) == 0xff &&
                ((buffer[3]) & 0xf0) == 0xe0) == 0)
        {
            fwrite(buffer, 512, 1, img);
            bytenum = fread(buffer, 512, 1, file);
            if (bytenum == 0)
            {
                return 0;
            }

        }

        fclose(img);
    }

    fclose(file);
    return 0;
}
