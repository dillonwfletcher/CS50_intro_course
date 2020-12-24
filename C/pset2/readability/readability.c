#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    string text; //init var for input text
    int n = 0; // init var to hold input text char length

    //ask for text input until input is given no null input allowed
    do
    {

        text = get_string("Text: ");
        n = strlen(text);

    }
    while (n == 0);

    //init vars for letter count, word count, and sentence count
    int ltr_cnt = 0;
    int wrd_cnt = 1; //assuming no space at input end, so start at 1 to account for final word
    int snt_cnt = 0;

    //loop through string input
    for (int i = 0; i < n; i++)
    {
        //check to see if char is a letter; add to letter count if so
        char ch = text[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        {
            ltr_cnt += 1;
        }

        //check to see if char is space indicating a word; add to word count
        else if (ch == ' ')
        {
            wrd_cnt += 1;
        }

        //check for punctuation; add to sent count; ok to not account for cases such as mr. or mrs.
        else if (ch == '.' || ch == '!' || ch == '?')
        {
            snt_cnt += 1;
        }

    }
    //use Coleman-Liau index to compute readability level
    //L is average num of letters per 100 words
    float L = ((float)ltr_cnt / wrd_cnt) * 100;

    //S is average num of sentences per 100 words
    float S = ((float)snt_cnt / wrd_cnt) * 100;

    //index formula
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    //use index value to determine what will be printed
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}