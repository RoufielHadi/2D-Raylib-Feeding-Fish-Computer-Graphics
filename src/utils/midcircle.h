#ifndef MIDCIRCLE_H
#define MIDCIRCLE_H

#include "raylib.h"

void Midcircle(int centerX, int centerY, int radius, Color color);
void MidcircleFilled(int centerX, int centerY, int radius, Color color);
void MidcircleThick(int centerX, int centerY, int radius, int thickness, Color color);
void MidcircleDashed(int centerX, int centerY, int radius, int dashLen, int gapLen, Color color);

#endif
