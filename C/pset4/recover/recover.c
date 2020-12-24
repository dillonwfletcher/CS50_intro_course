#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //Check to make sure there is only one command line arg
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    //Creating a new type to store a Byte of data; uint8_t represents an 8 bit unsigned int
    typedef uint8_t BYTE;

    //Open forensic image of memory card
    FILE *file = fopen(argv[1], "r");

    //Check to see if forensic image could be opened for reading
    if (!file)
    {
        printf("Image cannot be opened for reading.");
        return 1;
    }

    //Create a buffer to store 512MB chunk
    BYTE *buffer = malloc(sizeof(BYTE)*512);
    //Initialize a counter to keep track of number of images found
    int count = 0;
    //Initialize a string to be used for storing jpg filenames
    char* filename = malloc(sizeof(char)*7);

    //Loop through memmory card and look through 512MB chunks
    while(fread(buffer, sizeof(BYTE), 512, file) == 512)
    {
        //Check to see if first 4 bytes match header indicating a new jpg file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Create a new jpg file to write bytes to
            sprintf(filename, "%03i.jpg", count);
            //Open new jpg file as writable
            FILE *img = fopen(filename, "w");
            //Write bytes in buffer array to jpg file
            fwrite(buffer, sizeof(BYTE), 512, img);
            //Close file
            fclose(img);

            //Increase to keep track to count of jpg files found
            count++;
        }
        //If first 4 bytes are not header bytes and we have already found first jpg, we assign each 512MB chunk to most recent jpg file created until new jpg header found
        else if (count > 0)
        {
            //Reopen most previous created jpg file to append next 512MB chunk
            FILE *img = fopen(filename, "a");
            //Write bytes in buffer to jpg file
            fwrite(buffer, sizeof(BYTE), 512, img);
            //Close file
            fclose(img);
        }

    }

    //Close memory card file
    fclose(file);

    //Free up space from malloc use
    free(buffer);
    free(filename);
}
