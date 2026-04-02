#include "guppy.h"

void InitGuppy(Guppy *g, Vector2 pos) {
    g->pos = pos;
    g->vel = (Vector2){60, 0};
    g->wanderTarget = pos;
    g->time = 0;
    g->dir = 1;
    g->tailSwing = 0.0f;
    g->wanderTimer = 0.0f;
    g->finSwing = 0.0f;
    g->scale = 0.85f + (float)GetRandomValue(0, 20) / 100.0f;
    g->depth = 0.7f + (float)GetRandomValue(0, 30) / 100.0f;
    g->colorShift = (float)GetRandomValue(-10, 12);
    g->eatFlash = 0.0f;
    g->targetTimer = 0.0f;
    g->targetIndex = -1;
    g->deathTimer = 0.0f;
    g->hunger = 0;
    g->flipAngle = 0;
    g->active = true;
    g->state = GUPPY_NORMAL;
}

bool IsGuppyAlive(Guppy g) {
    return g.state != GUPPY_DEAD;
}

bool IsGuppyHungry(Guppy g) {
    return (g.active && g.state != GUPPY_DEAD && g.hunger > 0.5f);
}

void ResetGuppy(Guppy *g) {
    g->vel = (Vector2){60, 0};
    g->wanderTarget = g->pos;
    g->time = 0;
    g->dir = 1;
    g->tailSwing = 0.0f;
    g->wanderTimer = 0.0f;
    g->finSwing = 0.0f;
    g->eatFlash = 0.0f;
    g->targetTimer = 0.0f;
    g->targetIndex = -1;
    g->deathTimer = 0.0f;
    g->hunger = 0;
    g->flipAngle = 0;
    g->state = GUPPY_NORMAL;
    g->active = true;
}
