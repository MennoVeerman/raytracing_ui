#include "integrate.h"
void integrate(const float* field, const int width, const int height, float* integral)
{
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
            integral[j] += field[j+i*width];
}
