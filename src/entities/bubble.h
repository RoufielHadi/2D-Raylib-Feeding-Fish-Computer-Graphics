#ifndef BUBBLE_H
#define BUBBLE_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    BUBBLE_ACTIVE,
    BUBBLE_POPPED
} BubbleState;

typedef struct {
    Vector2 pos;
    Vector2 vel;

    float radius;
    float time;

    float alpha;
    bool active;
    BubbleState state;

} Bubble;

void InitBubble(Bubble *b, Vector2 pos);
bool IsBubbleActive(Bubble b);
void PopBubble(Bubble *b);
void ResetBubble(Bubble *b);

#endif
