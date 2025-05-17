
#include "riley4ext.h"


int main() {

    RGFW_window* win = RGFW_createWindow("Hello", (RGFW_rect){0, 0, 500, 500}, RGFW_CENTER);
	RSGL_init(RSGL_AREA(win->r.w, win->r.h), RGFW_getProcAddress);

    while (RGFW_window_shouldClose(win) == false) {

        while (RGFW_window_checkEvent(win)) {
			if (win->event.type == RGFW_quit) {
                break;
            }
		}

        RSGL_clear(RSGL_RGB(65, 65, 65));
        RSGL_rotate(RSGL_POINT3D(0, 0, 45));
        RSGL_drawRect(RSGL_RECT(150, 150, 200, 200), RSGL_RGB(255, 0, 0));

		RGFW_window_swapBuffers(win);
		RGFW_window_checkFPS(win, 0);

	}
	
	RSGL_free();
    RGFW_window_close(win);

    return 0;
}
