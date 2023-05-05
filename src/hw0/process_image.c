#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int c, int h, int w)
{
    if (h < 0)
        h = 0;
    if (h >= im.h)
        h = im.h - 1;
    if (w < 0)
        w = 0;
    if (w >= im.w)
        w = im.w - 1;

    return im.data[c * im.w * im.h + h * im.w + w];
}

void set_pixel(image im, int c, int h, int w, float v)
{
    if (c < 0 || c >= im.c)
        return;
    if (h < 0 || w < 0)
        return;
    if (h >= im.h || w >= im.w)
        return;
    
    im.data[c * im.w * im.h + h * im.w + w] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.c, im.h, im.w);
    for (int i = 0; i < im.c * im.h * im.w; i++)
        copy.data[i] = im.data[i];

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(1, im.h, im.w);
    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
        {
            float r = get_pixel(im, 0, i, j);
            float g = get_pixel(im, 1, i, j);
            float b = get_pixel(im, 2, i, j);
            float grayscale_weighted_sum = 0.299 * r + 0.587 * g + 0.114 * b;
            set_pixel(gray, 0, i, j, grayscale_weighted_sum);
        }
    
    return gray;
}

void shift_image(image im, int c, float v)
{
    assert(c >=0 && c <= 2);
    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
            set_pixel(im, c, i, j, get_pixel(im, c, i, j) + v);
}

void clamp_image(image im)
{
    for (int i = 0; i < im.c * im.h * im.w; i++)
    {
        if (im.data[i] < 0)
            im.data[i] = 0;
        if (im.data[i] > 1)
            im.data[i] = 1;
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
        {
            float R = get_pixel(im, 0, i, j);
            float G = get_pixel(im, 1, i, j);
            float B = get_pixel(im, 2, i, j);
            
            float V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);
            float C = V - m;
            float S = 0;
            if (V != 0)
                S = C / V;
            float H_prime = 0;
            if (C > 0 && V == R)
                H_prime = (G - B) / C;
            else if (C > 0 && V == G)
                H_prime = (B - R) / C + 2;
            else if (C > 0 && V == B)
                H_prime = (R - G) / C + 4;
            
            float H = H_prime / 6;
            if (H < 0)
                H += 1;
            
            set_pixel(im, 0, i, j, H);
            set_pixel(im, 1, i, j, S);
            set_pixel(im, 2, i, j, V);
        }
}

void hsv_to_rgb(image im)
{
    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
        {
            float H = get_pixel(im, 0, i, j);
            float S = get_pixel(im, 1, i, j);
            float V = get_pixel(im, 2, i, j);
            
            float H_prime = H * 6;
            float C = V * S;
            float R = 0, G = 0, B = 0;
            if (0 <= H_prime && H_prime < 1)
            {
                float X = 1 - (1 - H_prime);
                X *= C;
                R = C;
                G = X;
                B = 0;
            }
            else if (1 <= H_prime && H_prime < 2)
            {
                float X = 1 - (H_prime - 1);
                X *= C;
                R = X;
                G = C;
                B = 0;
            }
            else if (2 <= H_prime && H_prime < 3)
            {
                float X = 1 - (1 - (H_prime - 2));
                X *= C;
                R = 0;
                G = C;
                B = X;
            } 
            else if (3 <= H_prime && H_prime < 4)
            {
                float X = 1 - (H_prime - 2 - 1);
                X *= C;
                R = 0;
                G = X;
                B = C;
            }
            else if (4 <= H_prime && H_prime < 5)
            {
                float X = 1 - (1 - (H_prime - 4));
                X *= C;
                R = X;
                G = 0;
                B = C;
            }
            else if (5 <= H_prime && H_prime < 6)
            {
                float X = 1 - (H_prime - 4 - 1);
                X *= C;
                R = C;
                G = 0;
                B = X;
            }
            float m = V - C;

            set_pixel(im, 0, i, j, R + m);
            set_pixel(im, 1, i, j, G + m);
            set_pixel(im, 2, i, j, B + m);
        }
}

void scale_image(image im, int c, float v)
{
    assert(c >=0 && c <= 2);
    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
            set_pixel(im, c, i, j, get_pixel(im, c, i, j) * v);
    clamp_image(im);
}
