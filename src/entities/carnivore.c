#include "carnivore.h"

void InitCarnivore(Carnivore *c, Vector2 pos) {
    c->pos = pos;
    c->vel = (Vector2){80, 0};
    c->wanderTarget = pos;
    c->time = 0;
    c->dir = 1;
    c->tailSwing = 0.0f;
    c->wanderTimer = 0.0f;
    c->finSwing = 0.0f;
    c->scale = 0.92f + (float)GetRandomValue(0, 20) / 100.0f;
    c->depth = 0.75f + (float)GetRandomValue(0, 25) / 100.0f;
    c->colorShift = (float)GetRandomValue(-8, 10);
    c->eatFlash = 0.0f;
    c->deathTimer = 0.0f;
    c->hunger = 0;
    c->flipAngle = 0;
    c->active = true;
    c->state = CARNIVORE_NORMAL;
}

bool IsCarnivoreAlive(Carnivore c) {
    return c.state != CARNIVORE_DEAD;
}

bool IsCarnivoreHungry(Carnivore c) {
    return (c.active && c.state != CARNIVORE_DEAD && c.hunger > 0.6f);
}

void ResetCarnivore(Carnivore *c) {
    c->vel = (Vector2){80, 0};
    c->wanderTarget = c->pos;
    c->time = 0;
    c->dir = 1;
    c->tailSwing = 0.0f;
    c->wanderTimer = 0.0f;
    c->finSwing = 0.0f;
    c->eatFlash = 0.0f;
    c->deathTimer = 0.0f;
    c->hunger = 0;
    c->flipAngle = 0;
    c->state = CARNIVORE_NORMAL;
    c->active = true;
}
