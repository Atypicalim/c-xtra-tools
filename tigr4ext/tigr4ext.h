// tigr ext

#include "./tigr.h"

#if defined(TIGR_EXT_NO_DESKTOP) && defined(TIGR_EXT_NO_MOBILE)
#define TIGR_HEADLESS
#endif

#include "./src/tigr_internal.h"
#include "./src/tigr_bitmaps.c"
#include "./src/tigr_loadpng.c"
#include "./src/tigr_savepng.c"
#include "./src/tigr_inflate.c"
#include "./src/tigr_print.c"

#ifndef TIGR_HEADLESS
#include "./src/tigr_upscale_gl_vs.h"
#include "./src/tigr_upscale_gl_fs.h"
#include "./src/tigr_gl.c"
#endif

#ifndef TIGR_EXT_NO_DESKTOP
#include "./src/tigr_win.c"
#include "./src/tigr_osx.c"
#include "./src/tigr_linux.c"
#endif

#ifndef TIGR_EXT_NO_MOBILE
#include "./src/tigr_ios.c"
#include "./src/tigr_android.h"
#include "./src/tigr_android.c"
#endif

#include "./src/tigr_utils.c"

void tigrEllipse(Tigr *bmp, int x, int y, int rx, int ry, TPixel color);
void tigrFillEllipse(Tigr *bmp, int x, int y, int rx, int ry, TPixel color);

#include "tigr4ext.c"
