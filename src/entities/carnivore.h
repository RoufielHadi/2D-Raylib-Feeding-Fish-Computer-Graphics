#ifndef CARNIVORE_H
#define CARNIVORE_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    CARNIVORE_NORMAL,
    CARNIVORE_HUNGRY,
    CARNIVORE_DEAD
} CarnivoreState;

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
    CarnivoreState state;

} Carnivore;

void InitCarnivore(Carnivore *c, Vector2 pos);
bool IsCarnivoreAlive(Carnivore c);
bool IsCarnivoreHungry(Carnivore c);
void ResetCarnivore(Carnivore *c);

#endif
