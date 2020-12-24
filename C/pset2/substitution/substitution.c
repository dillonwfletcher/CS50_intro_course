#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //check to see that only 1 argument was given with command; argument should be 26 long alpha key
    if (argc == 2)
    {
        string key = argv[1]; //set given argument to string arg; this is our key
        int n = strlen(key); //get length of string

        //check to make sure given key is 26 char long
        if (n != 26)
        {
            printf("Error: Key must contain 26 letters.\n");
            return 1;
        }

        //iterate through arg to check that all chars are letters; if not send error
        for (int i = 0; i < n; i++)
        {
            if (isalpha(key[i]) == 0)
            {
                printf("Error: Key must contain 26 letters.\n");
                return 1;
            }

            //check to make sure there are no dublicate letters in key
            for (int j = 0; j < n; j++)
            {
                if (key[i] == key[j] && j != i)
                {
                    printf("Error: Duplicate letters in key.\n");
                    return 1;
                }
            }
        }

        //get text and text length user wants to encrypt
        string pt = get_string("plaintext: ");
        int len = strlen(pt);

        //go through text and shift letters based on key
        for (int i = 0; i < len; i++)
        {
            char c = pt[i];
            int index;
            //if text char is uppercase find place in alphabet 0-25 and replace with key value
            if (isupper(c))
            {
                index = c - 65;
                pt[i] = toupper(key[index]); //use to upper to ensure return upper value
            }
            //if text char is lowercase find place in alphabet 0-25 and replace with key value
            else if (islower(c))
            {
                index = c - 97;
                pt[i] = tolower(key[index]); //use tolower to ensure return lower value
            }

        }
        //print encrypted text
        printf("ciphertext: %s\n", pt);
        return 0;

    }
    //if no arg given or more than 1 arg given print error message
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}