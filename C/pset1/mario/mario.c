#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Get height that user wants for pyramid
    //height can be from 1 to 8 inclusively
    int ht;
    do
    {
        ht = get_int("Height: ");
    }
    while (ht < 1 || ht > 8);

    // i is being used to indicate which row we are creating. will create # of rows equal to height
    for (int i = 0; i < ht; i++)
    {
        //print the spaces needed to have correct indents on left side to make the ascending image of pyramid
        for (int j = ht - 1; j > i; j--)
        {
            printf(" ");
        }

        //print # needed to make left side of pyramid body
        //each row it will print an additonal # on left side
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // print the double spaces for the middle of pyramid on each row
        printf("  ");

        //print # needed to make right side of pyramid body
        //each row it will print an additonal # on right side
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        //go down a line to start next line
        printf("\n");

    }
}