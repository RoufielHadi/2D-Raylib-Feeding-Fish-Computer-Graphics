#ifndef DRAW_BUBBLE_H
#define DRAW_BUBBLE_H

#include "raylib.h"
#include "../entities/bubble.h"

void DrawBubble(const Bubble *b);
void DrawAllBubbles(const Bubble *arr, int count);
void WarmBubbleSpriteCache(void);

#endif

