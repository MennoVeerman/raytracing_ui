#include "helper_functions.h"

void get_color(const float x, int* rgb)
{
    const int idx = int(x*255)*3;
    rgb[0] = reds[idx];
    rgb[1] = reds[idx+1];
    rgb[2] = reds[idx+2];
}
