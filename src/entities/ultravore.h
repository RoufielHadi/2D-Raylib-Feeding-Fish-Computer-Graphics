#ifndef ULTRAVORE_H
#define ULTRAVORE_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    ULTRA_NORMAL,
    ULTRA_HUNGRY,
    ULTRA_DEAD
} UltravoreState;

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
    float deathTimer;

    float hunger;
    float flipAngle;
    bool active;
    UltravoreState state;

} Ultravore;

void InitUltravore(Ultravore *u, Vector2 pos);
bool IsUltravoreAlive(Ultravore u);
bool IsUltravoreHungry(Ultravore u);
void ResetUltravore(Ultravore *u);

#endif
