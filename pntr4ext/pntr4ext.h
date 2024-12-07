// tigr ext

#ifndef PNTR_EXT_HEADER
#define PNTR_EXT_HEADER

#ifdef PNTR_EXT_NO_CUTE
#define PNTR_NO_CUTE_PNG_IMPLEMENTATION
#else
#define PNTR_CUTE_PNG
#define PNTR_EXT_NO_STB
#endif

#ifdef PNTR_EXT_NO_STB
#define PNTR_NO_STB_IMAGE_IMPLEMENTATION
#define PNTR_NO_STB_IMAGE_WRITE_IMPLEMENTATION
#define PNTR_NO_STB_TRUETYPE_IMPLEMENTATION
#else
#define PNTR_STB_IMAGE
#endif

#if defined(PNTR_EXT_NO_STB) && defined(PNTR_EXT_NO_CUTE)
#define PNTR_NO_SAVE_IMAGE
#define PNTR_NO_LOAD_IMAGE
#endif

#ifdef PNTR_EXT_USE_ARGB
#define PNTR_PIXELFORMAT_ARGB
#else
#define PNTR_PIXELFORMAT_RGBA
#endif

#define PNTR_CUTE_PNG_H "../../others/cute_png.h"
#define PNTR_STB_IMAGE_H "../../others/stb_image.h"
#define PNTR_STB_IMAGE_WRITE_H "../../others/stb_image_write.h"

#define PNTR_TRUE_TYPE_H "../../others/stb_truetype.h"
#define PNTR_UTF8_PATH_H "./../others/utf8.h"
#define PNTR_FONT_PATH_H "./../others/font8x8_basic.h"

#define PNTR_IMPLEMENTATION
#define PNTR_ENABLE_DEFAULT_FONT
#include "pntr.h"

#include "pntr4ext.c"

#endif
