
#include "./tigr.h"

void tigrFillEllipse(Tigr* bmp, int x0, int y0, int rx, int ry, TPixel color) {
    if (rx <= 0 || ry <= 0) {
        return;
    }
    int rx2 = ry*ry;
    int ry2 = rx*rx;
    int rx2y2 = rx2*ry2;
    int tmp = 0;
    for(int y=-ry; y<=ry; y++) {
        for(int x=-rx; x<=rx; x++) {
            tmp = x*x*rx2+y*y*ry2;
            if(tmp <= rx2y2) {
                tigrPlot(bmp, x0+x, y0+y, color);
            }
        }
    }
}

void tigrEllipse(Tigr* bmp, int x0, int y0, int rx, int ry, TPixel color) {
    if (rx <= 0 || ry <= 0) {
        return;
    }
    int rx2 = ry*ry;
    int ry2 = rx*rx;
    int rx2y2 = rx2*ry2;
    int thickness = 1;
    int _rx2 = (ry - thickness)*(ry - thickness);
    int _ry2 = (rx - thickness)*(rx - thickness);
    int _rx2y2 = _rx2*_ry2;
    int tmp = 0;
    for(int y=-ry; y<=ry; y++) {
        for(int x=-rx; x<=rx; x++) {
            tmp = x*x*rx2+y*y*ry2;
            if(tmp <= rx2y2 && tmp > _rx2y2) {
                tigrPlot(bmp, x0+x, y0+y, color);
            }
        }
    }
}

