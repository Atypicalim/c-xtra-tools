// riley ext

#define RGFW_IMPLEMENTATION
#include "./src/RGFW.h"

#define RSGL_INT_DEFINED
#define RSGLDEF
#define RSGL_IMPLEMENTATION
#include "./src/RSGL.h"
#include "./src/RSGL_gl.h"

#if defined(RILEY_EXT_NO_SOMETHING)
#define RILEY_SOMELESS
#endif

#include "riley4ext.c"
