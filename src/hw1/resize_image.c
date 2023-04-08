#include <math.h>
#include "image.h"

float nn_interpolate(image im, int c, float h, float w)
{
    return get_pixel(im, c, round(h), round(w));
}

image nn_resize(image im, int h, int w)
{
    image resized_image = make_image(im.c, h, w);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            set_pixel(resized_image, 0, i, j, nn_interpolate(im, 0, i * (1.0 * im.h / h) - 0.5 + 0.5 * (1.0 * im.h / h), j * (1.0 * im.w / w) - 0.5 + 0.5 * (1.0 * im.w / w)));
            set_pixel(resized_image, 1, i, j, nn_interpolate(im, 1, i * (1.0 * im.h / h) - 0.5 + 0.5 * (1.0 * im.h / h), j * (1.0 * im.w / w) - 0.5 + 0.5 * (1.0 * im.w / w)));
            set_pixel(resized_image, 2, i, j, nn_interpolate(im, 2, i * (1.0 * im.h / h) - 0.5 + 0.5 * (1.0 * im.h / h), j * (1.0 * im.w / w) - 0.5 + 0.5 * (1.0 * im.w / w)));
        }
    return resized_image;
}

float bilinear_interpolate(image im, int c, float h, float w)
{
    int upper_left_i = floor(h);
    int upper_left_j = floor(w);

    int upper_right_i = floor(h);
    int upper_right_j = ceil(w);

    int lower_left_i = ceil(h);
    int lower_left_j = floor(w);

    int lower_right_i = ceil(h);
    int loewr_right_j = ceil(w);

    float upper_left_area = (h - upper_left_i) * (w - upper_left_j);
    float upper_right_area = (h - upper_right_i) * (upper_right_j - w);
    float lower_left_area = (lower_left_i - h) * (w - lower_left_j);
    float lower_right_area = (lower_right_i - h) * (loewr_right_j - w);

    float upper_left_pixel = get_pixel(im, c, upper_left_i, upper_left_j);
    float upper_right_pixel = get_pixel(im, c, upper_right_i, upper_right_j);
    float lower_left_pixel = get_pixel(im, c, lower_left_i, lower_left_j);
    float lower_right_pixel = get_pixel(im, c, lower_right_i, loewr_right_j);

    return upper_left_area * lower_right_pixel + upper_right_area * lower_left_pixel +
           lower_left_area * upper_right_pixel + lower_right_area * upper_left_pixel;
}

image bilinear_resize(image im, int h, int w)
{
    image resized_image = make_image(im.c, h, w);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            set_pixel(resized_image, 0, i, j, bilinear_interpolate(im, 0, i * (1.0 * im.h / h) - 0.5 + 0.5 * (1.0 * im.h / h), j * (1.0 * im.w / w) - 0.5 + 0.5 * (1.0 * im.w / w)));
            set_pixel(resized_image, 1, i, j, bilinear_interpolate(im, 1, i * (1.0 * im.h / h) - 0.5 + 0.5 * (1.0 * im.h / h), j * (1.0 * im.w / w) - 0.5 + 0.5 * (1.0 * im.w / w)));
            set_pixel(resized_image, 2, i, j, bilinear_interpolate(im, 2, i * (1.0 * im.h / h) - 0.5 + 0.5 * (1.0 * im.h / h), j * (1.0 * im.w / w) - 0.5 + 0.5 * (1.0 * im.w / w)));
        }
    return resized_image;
}

