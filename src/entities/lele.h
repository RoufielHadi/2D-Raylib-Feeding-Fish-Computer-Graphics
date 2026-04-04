/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef LELE_H
#define LELE_H

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

/* ======================
Fungsi InitCarnivore
=======================
Fungsi ini digunakan untuk menginisialisasi carnivore.
*/
void InitCarnivore(Carnivore *c, Vector2 pos);

/* ======================
Fungsi IsCarnivoreAlive
=======================
Fungsi ini digunakan untuk memeriksa carnivore alive.
*/
bool IsCarnivoreAlive(Carnivore c);

/* ======================
Fungsi IsCarnivoreHungry
=======================
Fungsi ini digunakan untuk memeriksa carnivore hungry.
*/
bool IsCarnivoreHungry(Carnivore c);

/* ======================
Fungsi ResetCarnivore
=======================
Fungsi ini digunakan untuk mengatur ulang carnivore.
*/
void ResetCarnivore(Carnivore *c);

#endif
