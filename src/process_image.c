#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

void main(){
float get_pixel(image im, int x, int y, int c)
{
    
    if (x < 0) x = 0;
    if (x >= im.w) x = im.w - 1;
    if (y < 0) y = 0;
    if (y >= im.h) y = im.h - 1;
    if (c < 0) c = 0;
    if (c >= im.c) c = im.c - 1;

    int index = c * im.h * im.w + y * im.w + x;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
   if (x < 0 || x >= im.w || y < 0 || y >= im.h || c < 0 || c >= im.c)
        return;

    int index = c * im.h * im.w + y * im.w + x;
    im.data[index] = v;
}


image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
       int data_size = im.w * im.h * im.c * sizeof(float);
    memcpy(copy.data, im.data, data_size);
    return copy;
}

image rgb_to_grayscale(image im)
{
     assert(im.c == 3);

    image gray = make_image(im.w, im.h, 1);

    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            float r = get_pixel(im, x, y, 0);
            float g = get_pixel(im, x, y, 1);
            float b = get_pixel(im, x, y, 2);

            float luma = 0.299 * r + 0.587 * g + 0.114 * b;

            set_pixel(gray, x, y, 0, luma);
        }
    }

    return gray;}

void shift_image(image im, int c, float v)
{
    int i, j;
    for (j = 0; j < im.h; ++j) {
        for (i = 0; i < im.w; ++i) {
            // Get pixel value
            float value = get_pixel(im, i, j, c);
            
            // Add the shift value
            value += v;
            
            // Set the shifted value
            set_pixel(im, i, j, c, value);
        }
    }
}


void clamp_image(image im)
{
    int i, j, k;
    for (k = 0; k < im.c; ++k) {
        for (j = 0; j < im.h; ++j) {
            for (i = 0; i < im.w; ++i) {
               
                float value = get_pixel(im, i, j, k);
                
               
                if (value < 0) {
                    value = 0;
                } else if (value > 1) {
                    value = 1;
                }
                
                
                set_pixel(im, i, j, k, value);
            }
        }
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
    int i;
    float r, g, b;
    float h, s, v;
    
    for (i = 0; i < im.w * im.h; ++i) {
        r = im.data[i];
        g = im.data[i + im.w * im.h];
        b = im.data[i + 2 * im.w * im.h];

        float max_val = fmaxf(r, fmaxf(g, b));
        float min_val = fminf(r, fminf(g, b));
        float delta = max_val - min_val;

        v = max_val;
        if (max_val != 0) {
            s = delta / max_val;
        } else {
            s = 0;
        }

        if (delta == 0) {
            h = 0;
        } else {
            if (max_val == r) {
                h = (g - b) / delta;
            } else if (max_val == g) {
                h = 2 + (b - r) / delta;
            } else {
                h = 4 + (r - g) / delta;
            }
            h /= 6;
            if (h < 0) {
                h += 1;
            }
        }

        im.data[i] = h;
        im.data[i + im.w * im.h] = s;
        im.data[i + 2 * im.w * im.h] = v;
    }
}


void hsv_to_rgb(image im)
{
    int i;
    float h, s, v;
    float r, g, b;
    
    for (i = 0; i < im.w * im.h; ++i) {
        h = im.data[i];
        s = im.data[i + im.w * im.h];
        v = im.data[i + 2 * im.w * im.h];

        float c = v * s;
        float x = c * (1 - fabsf(fmodf(6 * h, 2) - 1));
        float m = v - c;

        if (h < 1.0 / 6) {
            r = c;
            g = x;
            b = 0;
        } else if (h < 2.0 / 6) {
            r = x;
            g = c;
            b = 0;
        } else if (h < 3.0 / 6) {
            r = 0;
            g = c;
            b = x;
        } else if (h < 4.0 / 6) {
            r = 0;
            g = x;
            b = c;
        } else if (h < 5.0 / 6) {
            r = x;
            g = 0;
            b = c;
        } else {
            r = c;
            g = 0;
            b = x;
        }

        r = r+m;
        g = g+m;
        b += m;

        im.data[i] = r;
        im.data[i + im.w * im.h] = g;
        im.data[i + 2 * im.w * im.h] = b;
    }
}
}
