#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum = 0;
    for (int c = 0; c < im.c; c++)
        for (int i = 0; i < im.h; i++)
            for (int j = 0; j < im.w; j++)
            {
                sum += get_pixel(im, c, i, j);
            }
    
    for (int c = 0; c < im.c; c++)
        for (int i = 0; i < im.h; i++)
            for (int j = 0; j < im.w; j++)
            {
                set_pixel(im, c, i, j, get_pixel(im, c, i, j) * 1 / sum);
            }
}

image make_box_filter(int w)
{
    image im = make_image(1, w, w);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < w; j++)
        {
            set_pixel(im, 0, i, j, 1);
        }
    
    l1_normalize(im);
    return im;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(im.c == filter.c || (im.c != filter.c && filter.c == 1));
    image res;
    if (preserve == 1)
    {
        res = make_image(im.c, im.h, im.w);
        for (int c = 0; c < im.c; c++)
        for (int i = 0; i < im.h; i++)
            for (int j = 0; j < im.w; j++)
            {
                float sum = 0;
                for (int x = -filter.h / 2; x <= filter.h / 2; x++)
                    for (int y = -filter.w/2; y <= filter.w / 2; y++)
                    {
                        int filter_c = c;
                        if (filter.c == 1)
                            filter_c = 0;
                        sum += (get_pixel(im, c, i + x, j + y) * get_pixel(filter, filter_c, x + filter.h / 2, y + filter.w / 2));
                    }
                set_pixel(res, c, i, j, sum);
            }
    }
    else
    {
        res = make_image(1, im.h, im.w);
        for (int i = 0; i < im.h; i++)
            for (int j = 0; j < im.w; j++)
            {
                float sum = 0;
                for (int c = 0; c < im.c; c++)
                {
                    for (int x = -filter.h / 2; x <= filter.h / 2; x++)
                        for (int y = -filter.w/2; y <= filter.w / 2; y++)
                        {
                            int filter_c = c;
                            if (filter.c == 1)
                                filter_c = 0;
                            sum += (get_pixel(im, c, i + x, j + y) * get_pixel(filter, filter_c, x + filter.h / 2, y + filter.w / 2));
                        }
                }
                set_pixel(res, 0, i, j, sum);
            }    
    }

    return res;
}

image make_highpass_filter()
{
    image im = make_image(1, 3, 3);
    set_pixel(im, 0, 0, 0, 0);
    set_pixel(im, 0, 0, 1, -1);
    set_pixel(im, 0, 0, 2, 0);
    set_pixel(im, 0, 1, 0, -1);
    set_pixel(im, 0, 1, 1, 4);
    set_pixel(im, 0, 1, 2, -1);
    set_pixel(im, 0, 2, 0, 0);
    set_pixel(im, 0, 2, 1, -1);
    set_pixel(im, 0, 2, 2, 0);

    return im;
}

image make_sharpen_filter()
{
    image im = make_image(1, 3, 3);
    set_pixel(im, 0, 0, 0, 0);
    set_pixel(im, 0, 0, 1, -1);
    set_pixel(im, 0, 0, 2, 0);
    set_pixel(im, 0, 1, 0, -1);
    set_pixel(im, 0, 1, 1, 5);
    set_pixel(im, 0, 1, 2, -1);
    set_pixel(im, 0, 2, 0, 0);
    set_pixel(im, 0, 2, 1, -1);
    set_pixel(im, 0, 2, 2, 0);

    return im;
}

image make_emboss_filter()
{
    image im = make_image(1, 3, 3);
    set_pixel(im, 0, 0, 0, -2);
    set_pixel(im, 0, 0, 1, -1);
    set_pixel(im, 0, 0, 2, 0);
    set_pixel(im, 0, 1, 0, -1);
    set_pixel(im, 0, 1, 1, 1);
    set_pixel(im, 0, 1, 2, 1);
    set_pixel(im, 0, 2, 0, 0);
    set_pixel(im, 0, 2, 1, 1);
    set_pixel(im, 0, 2, 2, 2);

    return im;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: Use preserve: box filter, sharpen filter, emboss filter. Because using these filters we want to blur/sharpen/emboss the original images
//               it is like adding special effects to the images.
//         Don't use preserve: high-pass filter. Because it works as an edge-detector and we only need one channel to reflect the edges of
//               the whole image.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: We don't need to do post-processing for box filters but for all the other filters we need to clamp the final images to make sure
//          values are within rage 0 to 1. If the original image has values within the range than box filters will always return values within
//          the range. But all the other filters might result in values that are less than 0 or bigger than 1 so we want to clamp them.

image make_gaussian_filter(float sigma)
{
    int size = floor(6 * sigma);
    if (!(size & 1))
        size++;

    image im = make_image(1, size, size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            int x = i - size / 2;
            int y = j - size / 2;
            float value = exp(-1 * (x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            set_pixel(im, 0, i, j, value);
        }
    
    l1_normalize(im);
    return im;
}

image add_image(image a, image b)
{
    assert(a.c == b.c && a.h == b.h && a.w == b.w);
    image im = make_image(a.c, b.h, b.w);
    for (int c = 0; c < a.c; c++)
    for (int i = 0; i < a.h; i++)
        for (int j = 0; j < a.w; j++)
        {
            set_pixel(im, c, i, j, (get_pixel(a, c, i, j) + get_pixel(b, c, i, j)));
        }
    
    return im;
}

image sub_image(image a, image b)
{
    assert(a.c == b.c && a.h == b.h && a.w == b.w);
    image im = make_image(a.c, b.h, b.w);
    for (int c = 0; c < a.c; c++)
    for (int i = 0; i < a.h; i++)
        for (int j = 0; j < a.w; j++)
        {
            set_pixel(im, c, i, j, (get_pixel(a, c, i, j) - get_pixel(b, c, i, j)));
        }
    
    return im;
}

image make_gx_filter()
{
    image im = make_image(1, 3, 3);
    set_pixel(im, 0, 0, 0, -1);
    set_pixel(im, 0, 0, 1, 0);
    set_pixel(im, 0, 0, 2, 1);
    set_pixel(im, 0, 1, 0, -2);
    set_pixel(im, 0, 1, 1, 0);
    set_pixel(im, 0, 1, 2, 2);
    set_pixel(im, 0, 2, 0, -1);
    set_pixel(im, 0, 2, 1, 0);
    set_pixel(im, 0, 2, 2, 1);

    return im;
}

image make_gy_filter()
{
    image im = make_image(1, 3, 3);
    set_pixel(im, 0, 0, 0, -1);
    set_pixel(im, 0, 0, 1, -2);
    set_pixel(im, 0, 0, 2, -1);
    set_pixel(im, 0, 1, 0, 0);
    set_pixel(im, 0, 1, 1, 0);
    set_pixel(im, 0, 1, 2, 0);
    set_pixel(im, 0, 2, 0, 1);
    set_pixel(im, 0, 2, 1, 2);
    set_pixel(im, 0, 2, 2, 1);

    return im;
}

void feature_normalize(image im)
{
    float min_v = im.data[0];
    float max_v = im.data[0];
    for (int c = 0; c < im.c; c++)
        for (int i = 0; i < im.h; i++)
            for (int j = 0; j < im.w; j++)
            {
                float v = get_pixel(im, c, i, j);
                if (v > max_v)
                    max_v = v;
                if (v < min_v)
                    min_v = v;
            }
    
    for (int c = 0; c < im.c; c++)
        for (int i = 0; i < im.h; i++)
            for (int j = 0; j < im.w; j++)
            {
                if (max_v - min_v == 0)
                    set_pixel(im, c, i, j, 0);
                else
                    set_pixel(im, c, i, j, (get_pixel(im, c, i, j) - min_v) / (max_v - min_v));
            }
}

image *sobel_image(image im)
{
    image* res = calloc(2, sizeof(image));

    image gx = make_gx_filter();
    image gy = make_gy_filter();
    image Gx = convolve_image(im, gx, 0);
    image Gy = convolve_image(im, gy, 0);
    res[0] = make_image(1, im.h, im.w);
    res[1] = make_image(1, im.h, im.w);

    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
        {
            float x = get_pixel(Gx, 0, i, j);
            float y = get_pixel(Gy, 0, i, j);
            set_pixel(res[0], 0, i, j, sqrt(x * x + y * y));
            set_pixel(res[1], 0, i, j, atan2(y, x));
        }
    
    return res;
}

image colorize_sobel(image im)
{
    image res = make_image(3, im.h, im.w);
    image* sobel = sobel_image(im);
    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);
    for (int i = 0; i < im.h; i++)
        for (int j = 0; j < im.w; j++)
        {
            set_pixel(res, 0, i, j, get_pixel(sobel[1], 0, i, j));
            set_pixel(res, 1, i, j, get_pixel(sobel[0], 0, i, j));
            set_pixel(res, 2, i, j, get_pixel(sobel[0], 0, i, j));
        }
    hsv_to_rgb(res);

    return res;
}
