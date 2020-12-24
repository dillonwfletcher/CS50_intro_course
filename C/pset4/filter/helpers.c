#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int av = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float)3);

            image[i][j].rgbtBlue = av;
            image[i][j].rgbtGreen = av;
            image[i][j].rgbtRed = av;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image [i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make copy of original
    RGBTRIPLE c[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            c[i][j] = image[i][j];
        }
    }

    int bsum;
    int gsum;
    int rsum;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //top row
            if (i == 0)
            {
                if (j == 0)
                {
                    bsum = round((c[i][j].rgbtBlue + c[i][j + 1].rgbtBlue + c[i + 1][j].rgbtBlue + c[i + 1][j + 1].rgbtBlue) / (float)4);
                    gsum = round((c[i][j].rgbtGreen + c[i][j + 1].rgbtGreen + c[i + 1][j].rgbtGreen + c[i + 1][j + 1].rgbtGreen) / (float)4);
                    rsum = round((c[i][j].rgbtRed + c[i][j + 1].rgbtRed + c[i + 1][j].rgbtRed + c[i + 1][j + 1].rgbtRed) / (float)4);
                }
                else if (j == width - 1)
                {
                    bsum = round((c[i][j].rgbtBlue + c[i][j - 1].rgbtBlue + c[i + 1][j].rgbtBlue + c[i + 1][j - 1].rgbtBlue) / (float)4);
                    gsum = round((c[i][j].rgbtGreen + c[i][j - 1].rgbtGreen + c[i + 1][j].rgbtGreen + c[i + 1][j - 1].rgbtGreen) / (float)4);
                    rsum = round((c[i][j].rgbtRed + c[i][j - 1].rgbtRed + c[i + 1][j].rgbtRed + c[i + 1][j - 1].rgbtRed) / (float)4);
                }
                else
                {
                    bsum = round((c[i][j].rgbtBlue + c[i][j - 1].rgbtBlue + c[i][j + 1].rgbtBlue + c[i + 1][j].rgbtBlue + c[i+1][j-1].rgbtBlue + c[i+1][j+1].rgbtBlue) / (float)6);
                    gsum = round((c[i][j].rgbtGreen + c[i][j - 1].rgbtGreen + c[i][j + 1].rgbtGreen + c[i + 1][j].rgbtGreen + c[i+1][j-1].rgbtGreen + c[i+1][j+1].rgbtGreen) / (float)6);
                    rsum = round((c[i][j].rgbtRed + c[i][j - 1].rgbtRed + c[i][j + 1].rgbtRed + c[i + 1][j].rgbtRed + c[i+1][j-1].rgbtRed + c[i+1][j+1].rgbtRed) / (float)6);
                }
            }
            //bottom row
            else if (i == height - 1)
            {
                if (j == 0)
                {
                    bsum = round((c[i][j].rgbtBlue + c[i][j+1].rgbtBlue + c[i-1][j].rgbtBlue + c[i-1][j+1].rgbtBlue) / (float)4);
                    gsum = round((c[i][j].rgbtGreen + c[i][j+1].rgbtGreen + c[i-1][j].rgbtGreen + c[i-1][j+1].rgbtGreen) / (float)4);
                    rsum = round((c[i][j].rgbtRed + c[i][j+1].rgbtRed + c[i-1][j].rgbtRed + c[i-1][j+1].rgbtRed) / (float)4);
                }
                else if (j == width - 1)
                {
                    bsum = round((c[i][j].rgbtBlue + c[i][j-1].rgbtBlue + c[i-1][j].rgbtBlue + c[i-1][j-1].rgbtBlue) / (float)4);
                    gsum = round((c[i][j].rgbtGreen + c[i][j-1].rgbtGreen + c[i-1][j].rgbtGreen + c[i-1][j-1].rgbtGreen) / (float)4);
                    rsum = round((c[i][j].rgbtRed + c[i][j-1].rgbtRed + c[i-1][j].rgbtRed + c[i-1][j-1].rgbtRed) / (float)4);
                }
                else
                {
                    bsum = round((c[i][j].rgbtBlue + c[i][j-1].rgbtBlue + c[i][j+1].rgbtBlue + c[i-1][j].rgbtBlue + c[i-1][j-1].rgbtBlue + c[i-1][j+1].rgbtBlue) / (float)6);
                    gsum = round((c[i][j].rgbtGreen + c[i][j-1].rgbtGreen + c[i][j+1].rgbtGreen + c[i-1][j].rgbtGreen + c[i-1][j-1].rgbtGreen + c[i-1][j+1].rgbtGreen) / (float)6);
                    rsum = round((c[i][j].rgbtRed + c[i][j-1].rgbtRed + c[i][j+1].rgbtRed + c[i-1][j].rgbtRed + c[i-1][j-1].rgbtRed + c[i-1][j+1].rgbtRed) / (float)6);
                }
            }
            //left side excluding top and botto row
            else if (j == 0)
            {
                bsum = round((c[i][j].rgbtBlue + c[i][j+1].rgbtBlue + c[i+1][j].rgbtBlue + c[i-1][j].rgbtBlue + c[i+1][j+1].rgbtBlue + c[i-1][j+1].rgbtBlue) / (float)6);
                gsum = round((c[i][j].rgbtGreen + c[i][j+1].rgbtGreen + c[i+1][j].rgbtGreen + c[i-1][j].rgbtGreen + c[i+1][j+1].rgbtGreen + c[i-1][j+1].rgbtGreen) / (float)6);
                rsum = round((c[i][j].rgbtRed + c[i][j+1].rgbtRed + c[i+1][j].rgbtRed + c[i-1][j].rgbtRed + c[i+1][j+1].rgbtRed + c[i-1][j+1].rgbtRed) / (float)6);
            }
            else if (j == width - 1)
            {
                bsum = round((c[i][j].rgbtBlue + c[i][j-1].rgbtBlue + c[i+1][j].rgbtBlue + c[i-1][j].rgbtBlue + c[i-1][j-1].rgbtBlue + c[i+1][j-1].rgbtBlue) / (float)6);
                gsum = round((c[i][j].rgbtGreen + c[i][j-1].rgbtGreen + c[i+1][j].rgbtGreen + c[i-1][j].rgbtGreen + c[i-1][j-1].rgbtGreen + c[i+1][j-1].rgbtGreen) / (float)6);
                rsum = round((c[i][j].rgbtRed + c[i][j-1].rgbtRed + c[i+1][j].rgbtRed + c[i-1][j].rgbtRed + c[i-1][j-1].rgbtRed + c[i+1][j-1].rgbtRed) / (float)6);
            }
            else
            {
                bsum = round((c[i][j].rgbtBlue + c[i][j-1].rgbtBlue + c[i][j+1].rgbtBlue + c[i+1][j].rgbtBlue + c[i-1][j].rgbtBlue + c[i-1][j-1].rgbtBlue + c[i+1][j-1].rgbtBlue + c[i-1][j+1].rgbtBlue + c[i+1][j+1].rgbtBlue) / (float)9);
                gsum = round((c[i][j].rgbtGreen + c[i][j-1].rgbtGreen + c[i][j+1].rgbtGreen + c[i+1][j].rgbtGreen + c[i-1][j].rgbtGreen + c[i-1][j-1].rgbtGreen + c[i+1][j-1].rgbtGreen + c[i-1][j+1].rgbtGreen + c[i+1][j+1].rgbtGreen) / (float)9);
                rsum = round((c[i][j].rgbtRed + c[i][j-1].rgbtRed + c[i][j+1].rgbtRed + c[i+1][j].rgbtRed + c[i-1][j].rgbtRed + c[i-1][j-1].rgbtRed + c[i+1][j-1].rgbtRed + c[i-1][j+1].rgbtRed + c[i+1][j+1].rgbtRed) / (float)9);
            }

            image[i][j].rgbtBlue = bsum;
            image[i][j].rgbtGreen = gsum;
            image[i][j].rgbtRed = rsum;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //make copy of original
    RGBTRIPLE c[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            c[i][j] = image[i][j];
        }
    }

    //to get the edge effect will use sobel operator
    //  Gx          Gy
    //-1 0 1     -1 -2 -1
    //-2 0 2      0  0  0
    //-1 0 1      1  2  1

    int Gxb, Gxg, Gxr, Gyb, Gyg, Gyr;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //top row
            if (i == 0)
            {
                if (j == 0)
                {
                    Gxb = c[i][j+1].rgbtBlue*2 + c[i+1][j+1].rgbtBlue;
                    Gxg = c[i][j+1].rgbtGreen*2 + c[i+1][j+1].rgbtGreen;
                    Gxr = c[i][j+1].rgbtRed*2 + c[i+1][j+1].rgbtRed;

                    Gyb = c[i+1][j].rgbtBlue*2 + c[i+1][j+1].rgbtBlue;
                    Gyg = c[i+1][j].rgbtGreen*2 + c[i+1][j+1].rgbtGreen;
                    Gyr = c[i+1][j].rgbtRed*2 + c[i+1][j+1].rgbtRed;

                }
                else if (j == width - 1)
                {
                    Gxb = c[i][j-1].rgbtBlue*(-2) + c[i+1][j-1].rgbtBlue*(-1);
                    Gxg = c[i][j-1].rgbtGreen*(-2) + c[i+1][j-1].rgbtGreen*(-1);
                    Gxr = c[i][j-1].rgbtRed*(-2) + c[i+1][j-1].rgbtRed*(-1);

                    Gyb = c[i+1][j].rgbtBlue*2 + c[i+1][j-1].rgbtBlue;
                    Gyg = c[i+1][j].rgbtGreen*2 + c[i+1][j-1].rgbtGreen;
                    Gyr = c[i+1][j].rgbtRed*2 + c[i+1][j-1].rgbtRed;
                }
                else
                {
                    Gxb = c[i][j-1].rgbtBlue*(-2) + c[i][j+1].rgbtBlue*2 + c[i+1][j-1].rgbtBlue*(-1) + c[i+1][j+1].rgbtBlue;
                    Gxg = c[i][j-1].rgbtGreen*(-2) + c[i][j+1].rgbtGreen*2 + c[i+1][j-1].rgbtGreen*(-1) + c[i+1][j+1].rgbtGreen;
                    Gxr = c[i][j-1].rgbtRed*(-2) + c[i][j+1].rgbtRed*2 + c[i+1][j-1].rgbtRed*(-1) + c[i+1][j+1].rgbtRed;

                    Gyb = c[i+1][j].rgbtBlue*2 + c[i+1][j-1].rgbtBlue + c[i+1][j+1].rgbtBlue;
                    Gyg = c[i+1][j].rgbtGreen*2 + c[i+1][j-1].rgbtGreen + c[i+1][j+1].rgbtGreen;
                    Gyr = c[i+1][j].rgbtRed*2 + c[i+1][j-1].rgbtRed + c[i+1][j+1].rgbtRed;
                }
            }
            //bottom row
            else if (i == height - 1)
            {
                if (j == 0)
                {
                    Gxb = c[i][j+1].rgbtBlue*2 + c[i-1][j+1].rgbtBlue;
                    Gxg = c[i][j+1].rgbtGreen*2 + c[i-1][j+1].rgbtGreen;
                    Gxr = c[i][j+1].rgbtRed*2 + c[i-1][j+1].rgbtRed;

                    Gyb = c[i-1][j].rgbtBlue*(-2) + c[i-1][j+1].rgbtBlue*(-1);
                    Gyg = c[i-1][j].rgbtGreen*(-2) + c[i-1][j+1].rgbtGreen*(-1);
                    Gyr = c[i-1][j].rgbtRed*(-2) + c[i-1][j+1].rgbtRed*(-1);
                }
                else if (j == width - 1)
                {
                    Gxb = c[i][j-1].rgbtBlue*(-2) + c[i-1][j-1].rgbtBlue*(-1);
                    Gxg = c[i][j-1].rgbtGreen*(-2) + c[i-1][j-1].rgbtGreen*(-1);
                    Gxr = c[i][j-1].rgbtRed*(-2) + c[i-1][j-1].rgbtRed*(-1);

                    Gyb = c[i-1][j].rgbtBlue*(-2) + c[i-1][j-1].rgbtBlue*(-1);
                    Gyg = c[i-1][j].rgbtGreen*(-2) + c[i-1][j-1].rgbtGreen*(-1);
                    Gyr = c[i-1][j].rgbtRed*(-2) + c[i-1][j-1].rgbtRed*(-1);
                }
                else
                {
                    Gxb = c[i][j-1].rgbtBlue*(-2) + c[i][j+1].rgbtBlue*2 + c[i-1][j-1].rgbtBlue*(-1) + c[i-1][j+1].rgbtBlue;
                    Gxg = c[i][j-1].rgbtGreen*(-2) + c[i][j+1].rgbtGreen*2 + c[i-1][j-1].rgbtGreen*(-1) + c[i-1][j+1].rgbtGreen;
                    Gxr = c[i][j-1].rgbtRed*(-2) + c[i][j+1].rgbtRed*2 + c[i-1][j-1].rgbtRed*(-1) + c[i-1][j+1].rgbtRed;

                    Gyb = c[i-1][j].rgbtBlue*(-2) + c[i-1][j-1].rgbtBlue*(-1) + c[i-1][j+1].rgbtBlue*(-1);
                    Gyg = c[i-1][j].rgbtGreen*(-2) + c[i-1][j-1].rgbtGreen*(-1) + c[i-1][j+1].rgbtGreen*(-1);
                    Gyr = c[i-1][j].rgbtRed*(-2) + c[i-1][j-1].rgbtRed*(-1) + c[i-1][j+1].rgbtRed*(-1);
                }
            }
            //left side excluding top and botto row
            else if (j == 0)
            {
                Gxb = c[i][j+1].rgbtBlue*2 + c[i+1][j+1].rgbtBlue + c[i-1][j+1].rgbtBlue;
                Gxg = c[i][j+1].rgbtGreen*2 + c[i+1][j+1].rgbtGreen + c[i-1][j+1].rgbtGreen;
                Gxr = c[i][j+1].rgbtRed*2 + c[i+1][j+1].rgbtRed + c[i-1][j+1].rgbtRed;

                Gyb = c[i+1][j].rgbtBlue*2 + c[i-1][j].rgbtBlue*(-2) + c[i+1][j+1].rgbtBlue + c[i-1][j+1].rgbtBlue*(-1);
                Gyg = c[i+1][j].rgbtGreen*2 + c[i-1][j].rgbtGreen*(-2) + c[i+1][j+1].rgbtGreen + c[i-1][j+1].rgbtGreen*(-1);
                Gyr = c[i+1][j].rgbtRed*2 + c[i-1][j].rgbtRed*(-2) + c[i+1][j+1].rgbtRed + c[i-1][j+1].rgbtRed*(-1);
            }
            //right side excluding top and bottom rows
            else if (j == width - 1)
            {
                Gxb = c[i][j-1].rgbtBlue*(-2) + c[i-1][j-1].rgbtBlue*(-1) + c[i+1][j-1].rgbtBlue*(-1);
                Gxg = c[i][j-1].rgbtGreen*(-2) + c[i-1][j-1].rgbtGreen*(-1) + c[i+1][j-1].rgbtGreen*(-1);
                Gxr = c[i][j-1].rgbtRed*(-2) + c[i-1][j-1].rgbtRed*(-1) + c[i+1][j-1].rgbtRed*(-1);

                Gyb = c[i+1][j].rgbtBlue*2 + c[i-1][j].rgbtBlue*(-2) + c[i-1][j-1].rgbtBlue*(-1) + c[i+1][j-1].rgbtBlue;
                Gyg = c[i+1][j].rgbtGreen*2 + c[i-1][j].rgbtGreen*(-2) + c[i-1][j-1].rgbtGreen*(-1) + c[i+1][j-1].rgbtGreen;
                Gyr = c[i+1][j].rgbtRed*2 + c[i-1][j].rgbtRed*(-2) + c[i-1][j-1].rgbtRed*(-1) + c[i+1][j-1].rgbtRed;
            }
            //not on edge of image
            else
            {
                Gxb = c[i][j-1].rgbtBlue*(-2) + c[i][j+1].rgbtBlue*2 + c[i-1][j-1].rgbtBlue*(-1) + c[i+1][j-1].rgbtBlue*(-1) + c[i-1][j+1].rgbtBlue + c[i+1][j+1].rgbtBlue;
                Gxg = c[i][j-1].rgbtGreen*(-2) + c[i][j+1].rgbtGreen*2 + c[i-1][j-1].rgbtGreen*(-1) + c[i+1][j-1].rgbtGreen*(-1) + c[i-1][j+1].rgbtGreen + c[i+1][j+1].rgbtGreen;
                Gxr = c[i][j-1].rgbtRed*(-2) + c[i][j+1].rgbtRed*2 + c[i-1][j-1].rgbtRed*(-1) + c[i+1][j-1].rgbtRed*(-1) + c[i-1][j+1].rgbtRed + c[i+1][j+1].rgbtRed;

                Gyb = c[i+1][j].rgbtBlue*2 + c[i-1][j].rgbtBlue*(-2) + c[i-1][j-1].rgbtBlue*(-1) + c[i+1][j-1].rgbtBlue + c[i-1][j+1].rgbtBlue*(-1) + c[i+1][j+1].rgbtBlue;
                Gyg = c[i+1][j].rgbtGreen*2 + c[i-1][j].rgbtGreen*(-2) + c[i-1][j-1].rgbtGreen*(-1) + c[i+1][j-1].rgbtGreen + c[i-1][j+1].rgbtGreen*(-1) + c[i+1][j+1].rgbtGreen;
                Gyr = c[i+1][j].rgbtRed*2 + c[i-1][j].rgbtRed*(-2) + c[i-1][j-1].rgbtRed*(-1) + c[i+1][j-1].rgbtRed + c[i-1][j+1].rgbtRed*(-1) + c[i+1][j+1].rgbtRed;
            }

            //calculate sobel filter value which is sqrt(Gx^2 + Gy^2)
            int sobel_b = round(sqrt(pow(Gxb,2) + pow(Gyb,2)));
            int sobel_g = round(sqrt(pow(Gxg,2) + pow(Gyg,2)));
            int sobel_r = round(sqrt(pow(Gxr,2) + pow(Gyr,2)));

            //check to see if value exceeds 255 if so set to pixel color value to 255
            if (sobel_b > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sobel_b;
            }

            if (sobel_g > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sobel_g;
            }

            if (sobel_r > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sobel_r;
            }

        }
    }
    return;
}
