#include "ultravore.h"

void InitUltravore(Ultravore *u, Vector2 pos) {
    u->pos = pos;
    u->vel = (Vector2){70, 0};
    u->wanderTarget = pos;
    u->time = 0;
    u->dir = 1;
    u->tailSwing = 0.0f;
    u->wanderTimer = 0.0f;
    u->finSwing = 0.0f;
    u->scale = 1.00f + (float)GetRandomValue(0, 18) / 100.0f;
    u->depth = 0.78f + (float)GetRandomValue(0, 22) / 100.0f;
    u->colorShift = (float)GetRandomValue(-6, 8);
    u->eatFlash = 0.0f;
    u->deathTimer = 0.0f;
    u->hunger = 0;
    u->flipAngle = 0;
    u->active = true;
    u->state = ULTRA_NORMAL;
}

bool IsUltravoreAlive(Ultravore u) {
    return u.state != ULTRA_DEAD;
}

bool IsUltravoreHungry(Ultravore u) {
    return (u.active && u.state != ULTRA_DEAD && u.hunger > 0.7f);
}

void ResetUltravore(Ultravore *u) {
    u->vel = (Vector2){70, 0};
    u->wanderTarget = u->pos;
    u->time = 0;
    u->dir = 1;
    u->tailSwing = 0.0f;
    u->wanderTimer = 0.0f;
    u->finSwing = 0.0f;
    u->eatFlash = 0.0f;
    u->deathTimer = 0.0f;
    u->hunger = 0;
    u->flipAngle = 0;
    u->state = ULTRA_NORMAL;
    u->active = true;
}
