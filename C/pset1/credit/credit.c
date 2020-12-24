#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //get card number from user... cont until only numeric chars entered
    long cn;
    do
    {
        cn = get_long("Number: ");
    }
    while (cn % 1 == cn);

    int store[16];

    //enter card number into an array in reverse order (last number of card is first in array)
    for (int i = 0; i < 16; i++)
    {
        store[i] = cn % 10;
        cn = cn / 10;
    }

    //iterate through array to conduct luhns algo
    int chsum;
    for (int i = 0; i < 16; i++)
    {
        if (i % 2 != 0)
        {
            int num = store[i] * 2;
            if (num > 9)
            {
                int num1 = num % 10;
                int num2 = num / 10 % 10;

                chsum = chsum + num1 + num2;
            }
            else
            {
                chsum = chsum + num;
            }

        }
        else
        {
            chsum = chsum + store[i];
        }
    }

    //use chsum to validate card
    //if chsum modulo 10 congruent to 0 then card is valid; access first number of card to identify card type
    if (chsum % 10 == 0)
    {
        //get first num of card whether card num length is 13, 15, or 16 digits
        int count = 15;
        int fn = 0;
        while (fn == 0)
        {
            fn = store[count];
            count--;
        }
        //all visa cards start with 4
        if (fn == 4)
        {
            printf("VISA\n");
        }
        //all amex cards start with 3
        else if (fn == 3)
        {
            printf("AMEX\n");
        }
        // mastercards start with 5 and other numbers. for this problem we are only concerned with visa, amex,
        // and mastercard, so if not amex or visa then we infer valid number is mastercard
        else
        {
            printf("MASTERCARD\n");
        }

    }
    //if luhns theorom indicates invalid number we output invalid
    else
    {
        printf("INVALID\n");
    }
}