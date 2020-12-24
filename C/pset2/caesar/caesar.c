#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //check to see that only 1 argument was given with command; argument should be num key
    if (argc == 2)
    {
        string arg = argv[1]; //set given argument to string arg; this is our key
        int n = strlen(arg); //get length of string

        //iterate through arg to check that all chars are digits; if not send error
        for (int i = 0; i < n; i++)
        {
            if (isdigit(arg[i]) == 0)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }

        //convert str arg to int; modulo 26 to get rid of excess cycles through alphabet
        int key = atoi(arg) % 26;

        //get text and text length user wants to encrypt
        string pt = get_string("plaintext: ");
        int len = strlen(pt);

        //go through text and shift letters based on key
        for (int i = 0; i < len; i++)
        {
            char c = pt[i];

            //97 is ascii for 'a'
            if (c >= 'a' && c <= 'z')
            {
                c = (c + key - 97) % 26 + 97;
            }

            //65 is ascii for 'A'
            else if (c >= 'A' && c <= 'Z')
            {
                c = (c + key - 65) % 26 + 65;
            }

            //assign encrypted character to ith position
            pt[i] = c;
        }
        //print encrypted text
        printf("ciphertext: %s\n", pt);

    }
    //if no arg given or more than 1 arg given print error message
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}