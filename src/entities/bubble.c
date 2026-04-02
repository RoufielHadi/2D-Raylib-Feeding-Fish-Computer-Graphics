#include "bubble.h"

void InitBubble(Bubble *b, Vector2 pos) {
    b->pos = pos;
    b->vel = (Vector2){0, (float)(-30 - GetRandomValue(0,30))};
    b->radius = (float)(GetRandomValue(4, 12));
    b->time = 0;
    b->alpha = (unsigned char)GetRandomValue(140, 240);
    b->active = true;
    b->state = BUBBLE_ACTIVE;
}

bool IsBubbleActive(Bubble b) {
    return b.active && b.state == BUBBLE_ACTIVE;
}

void PopBubble(Bubble *b) {
    b->active = false;
    b->state = BUBBLE_POPPED;
}

void ResetBubble(Bubble *b) {
    b->time = 0;
    b->alpha = 180;
    b->active = true;
    b->state = BUBBLE_ACTIVE;
}
