#ifndef RAYTRACER_H
#define RAYTRACER_H
#include <vector>

void trace_ray(const float* tau, const float* ssa, const float g, const int* cld_mask,
               const int* size, const float albedo, const float sza_rad,
               const float cloud_clear_frac, const float k_null,
               const int n_photon, int* sfc_dir, int* sfc_dif, const int nx);
#endif // RAYTRACER_H
