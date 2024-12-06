
#include "pntr4ext.h"

int main() {

    pntr_image* image = pntr_new_image(200, 200);
    pntr_draw_ellipse_fill(image, 100, 100, 80, 50, PNTR_GREEN);
    pntr_save_image(image, "test.png");
    pntr_unload_image(image);

    return 0;
}
