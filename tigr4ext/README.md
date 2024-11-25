# TIGR - TIny GRaphics library

https://github.com/erkkah/tigr

TIGR is a tiny cross-platform graphics library,
providing a unified API for Windows, macOS, Linux, iOS and Android.

TIGR's core is a simple framebuffer library.
On top of that, we provide a few helpers for the common tasks that 2D programs generally need:

 - Bitmap-backed windows.
 - Direct access to bitmaps, no locking.
 - Basic drawing helpers (plot, line, blitter).
 - Text output using bitmap fonts.
 - Mouse, touch and keyboard input.
 - MyTigr_PNG loading and saving.
 - Easy pixel shader access.

There are no additional libraries to include; everything is baked right into your program.

## How to program

```C
#include "tigr.h"

int main(int argc, char *argv[])
{
    Tigr *screen = tigrWindow(320, 240, "Hello", 0);
    while (!tigrClosed(screen))
    {
        tigrClear(screen, tigrRGB(0x80, 0x90, 0xa0));
        tigrPrint(screen, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), "Hello, world.");
        tigrUpdate(screen);
    }
    tigrFree(screen);
    return 0;
}
```

## How to setup


TIGR is supplied as a single .c and corresponding .h file.

To use it, you just drop them right into your project.

1. Grab  **tigr.c** and **tigr.h**
2. Throw them into your project.
3. Link with commands
4. You're done!

## How to Link

 - -lopengl32 and -lgdi32 on Windows
 - -framework OpenGL and -framework Cocoa on macOS
 - -lGLU -lGL -lX11 on Linux
