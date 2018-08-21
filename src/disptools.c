#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disptools.h"

struct disptools_error_state disptools_error = {0, 0, "", "", "", ""};

int get_float_type_size(void) {
    return 8 * sizeof (FLOATING);
}

Image new_image(
        const size_t nd,
        const size_t nx,
        const size_t ny,
        const size_t nz,
        const FLOATING dx,
        const FLOATING dy,
        const FLOATING dz
        )
{
    Image img = {nd, nx, ny, nz, dx, dy, dz, NULL};
    img.data = (FLOATING*) calloc(nd * nx * ny * nz, sizeof (FLOATING));
    DISPTOOLS_SET_ERROR(!img.data, strerror(errno));
    return img;
}

Mask new_mask(
        const size_t nx,
        const size_t ny,
        const size_t nz
        )
{
    Mask mask = {nx, ny, nz, NULL};
    mask.data = (bool*) calloc(nx * ny * nz, sizeof (bool));
    DISPTOOLS_SET_ERROR(!mask.data, strerror(errno));
    return mask;
}

void delete_image(Image *img)
{
    if (img->data) {
        free(img->data);
        img->data = NULL;
    }
}

void delete_mask(Mask *mask)
{
    if (mask->data) {
        free(mask->data);
        mask->data = NULL;
    }
}

void print_image_info(const Image img)
{
    printf("nd: %ld\n"
           "nx: %ld\n"
           "ny: %ld\n"
           "nz: %ld\n"
           "dx: %f\n"
           "dy: %f\n"
           "dz: %f\n",
           img.nd,
           img.nx, img.ny, img.nz,
           img.dx, img.dy, img.dz
           );
}

Mask mask_from_image(const Image img)
{
    assert(1 == img.nd && "Mask image must be scalar");

    Mask mask = new_mask(img.nx, img.ny, img.nz);
    for (size_t z = 0; z < img.nz; ++z) {
        for (size_t y = 0; y < img.ny; ++y) {
            for (size_t x = 0; x < img.nx; ++x) {
                __(mask, x, y, z) = !!__(img, x, y, z);
            }
        }
    }
    return mask;
}
