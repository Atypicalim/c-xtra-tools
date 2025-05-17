# RSGL Riley's Simple Graphics Library

https://github.com/ColleagueRiley/RSGL

RSGL is a modular simple-to-use cross-platform graphics library. It attempts to combine the freedoms of low libraries with modern C techniques. Thus offering simplicity and convenience.

## Examples

```c
#define RSGL_IMPLEMENTATION
#include "RSGL.h"

int main() {
    RSGL_window* win = RSGL_createWindow("name", RSGL_RECT(0, 0, 500, 500), RSGL_CENTER);

    bool running = true;

    while (running) {
      while (RSGL_window_checkEvent(win)) {
          if (win->event.type == RSGL_quit) {
            running = false;
            break;
          }
      }

      RSGL_drawRect((RSGL_rect){200, 200, 200, 200}, RSGL_RGB(255, 0, 0));
      RSGL_window_clear(win, RSGL_RGB(255, 255, 255));
    }

    RSGL_window_close(win);
}
```

## Compiling

* Linux:

    `gcc <file.c> -lX11 -lGLX -lm -o example`
* Windows :

    `gcc <file.c> -lopengl32 -lgdi32 -lshell32 -o example`
* MacOS :

    `gcc <file.c> -lm -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo -o example`


## Backup

* Revision: ee36b38f35503efbfd158222f8f2daad13bea32b
* Author: ColleagueRiley
* Date: 2024/12/4 23:36:33
