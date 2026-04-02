#ifndef GUPPY_H
#define GUPPY_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    GUPPY_NORMAL,
    GUPPY_HUNGRY,
    GUPPY_DEAD
} GuppyState;

typedef struct {
    Vector2 pos;
    Vector2 vel;
    Vector2 wanderTarget;

    float time;
    float dir;
    float tailSwing;
    float wanderTimer;
    float finSwing;
    float scale;
    float depth;
    float colorShift;
    float eatFlash;
    float targetTimer;
    int targetIndex;
    float deathTimer;

    float hunger;
    float flipAngle;

    bool active;

    GuppyState state;

} Guppy;

void InitGuppy(Guppy *g, Vector2 pos);

bool IsGuppyAlive(Guppy g);
bool IsGuppyHungry(Guppy g);
void ResetGuppy(Guppy *g);

#endif
