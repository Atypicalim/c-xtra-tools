# PNTR - Image manipulation library for C99 or C++, with a focus on ease-of-use.

## Usage

``` c
#define PNTR_PIXELFORMAT_RGBA
#define PNTR_IMPLEMENTATION
#include "pntr.h"

int main() {
    pntr_image* image = pntr_new_image(200, 200);
    pntr_draw_circle_fill(image, 100, 100, 80, PNTR_RED);
    pntr_save_image(image, "output.png");
    pntr_unload_image(image);

    return 0;
}
```

### Configuration

Add these defines prior to including `pntr.h` to modify how it functions.

| Define | Description |
| --- | --- |
| `PNTR_IMPLEMENTATION` | Define this in one of your `.c` or `.cpp` files before including `pntr.h` |
| `PNTR_PIXELFORMAT_RGBA` | Use the `RGBA` format |
| `PNTR_PIXELFORMAT_ARGB` | Use the `ARGB` pixel format |
| `PNTR_ENABLE_DEFAULT_FONT` | Enables the default font |
| `PNTR_ENABLE_MATH` | Enables use of C's standard [`math.h`](https://en.cppreference.com/w/c/numeric/math) linked library, rather than using the built in math functions |
| `PNTR_ENABLE_TTF` | Enables support for loading [TrueType fonts](https://en.wikipedia.org/wiki/TrueType_fonts) |
| `PNTR_ENABLE_UTF8` | Enables [UTF-8](https://en.wikipedia.org/wiki/UTF-8) support for font loading and text rendering |
| `PNTR_LOAD_FILE` | Callback to use when asked to load a file in `pntr_load_file()`. By default, will use `stdio.h`. |
| `PNTR_LOAD_IMAGE_FROM_MEMORY` | Callback to use when loading an image from memory via `pntr_load_image_from_memory()`. By default, will use  [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) |
| `PNTR_SAVE_FILE` | Callback to use when saving a file via `pntr_save_file()`. By default, uses `stdio.h` |
| `PNTR_SAVE_IMAGE_TO_MEMORY` | Callback to use when saving an image to memory via `pntr_save_image_to_memory()`. By default, will use [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h) |
| `PNTR_NO_ALPHABLEND` | Skips alpha blending when drawing pixels |
| `PNTR_NO_STDIO` | Will disable the standard file loading/saving calls for `PNTR_LOAD_FILE` and `PNTR_SAVE_FILE` |
| `PNTR_NO_SAVE_IMAGE` | Disables the default behavior of image saving |
| `PNTR_NO_LOAD_IMAGE` | Disables the default behavior of image loading |
| `PNTR_NO_CUTE_PNG_IMPLEMENTATION` | Skips defining `CUTE_PNG_IMPLEMENTATION`. Useful if you're using cute_png elsewhere |
| `PNTR_NO_STB_TRUETYPE_IMPLEMENTATION` | Skips defining `STB_TRUETYPE_IMPLEMENTATION`. Useful if you're using stb_truetype elsewhere |
| `PNTR_NO_STB_IMAGE_WRITE_IMPLEMENTATION` | Skips defining `STB_IMAGE_WRITE_IMPLEMENTATION`. useful if you're using stb_image_write elsewhere |
| `PNTR_NO_STB_IMAGE_IMPLEMENTATION` | Skips defining `STB_IMAGE_IMPLEMENTATION`. useful if you're using stb_image_write elsewhere |
