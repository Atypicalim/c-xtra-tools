
#ifndef PNTR_EXT_SOURCE
#define PNTR_EXT_SOURCE

#include "./pntr4ext.h"

bool pntr4ext_convert_pixels(pntr_image* image, unsigned int* size, void **data, pntr_pixelformat pixelFormat) {
    if (image == NULL) {
        pntr_set_error(PNTR_ERROR_INVALID_ARGS);
        return false;
    }
    //
    int pixelSize = pntr_get_pixel_data_size(1, 1, pixelFormat);
    if (pixelSize <= 0) {
        pntr_set_error(PNTR_ERROR_INVALID_ARGS);
        return false;
    }
    //
    int imageSize = pntr_get_pixel_data_size(image->width, image->height, pixelFormat);
    if (imageSize <= 0) {
        pntr_set_error(PNTR_ERROR_UNKNOWN);
        return false;
    }
    //
    *data = PNTR_REALLOC(*data, (size_t)imageSize);
    if (*data == NULL) {
        pntr_set_error(PNTR_ERROR_NO_MEMORY);
        return false;
    }
    //
    int i = 0;
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            pntr_set_pixel_color(
                ((unsigned char*)*data) + (i++ * pixelSize),
                pixelFormat,
                PNTR_PIXEL(image, x, y)
            );
        }
    }
    //
    if (size != NULL) {
        *size = (unsigned int)imageSize;
    }
    return true;
}

void pntr4ext_free_pixels(void **data) {
    free(*data);
}

#endif
