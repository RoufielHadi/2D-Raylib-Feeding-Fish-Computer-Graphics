/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef CERE_H
#define CERE_H

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

/* ======================
Fungsi InitGuppy
=======================
Fungsi ini digunakan untuk menginisialisasi guppy.
*/
void InitGuppy(Guppy *g, Vector2 pos);

/* ======================
Fungsi IsGuppyAlive
=======================
Fungsi ini digunakan untuk memeriksa guppy alive.
*/
bool IsGuppyAlive(Guppy g);

/* ======================
Fungsi IsGuppyHungry
=======================
Fungsi ini digunakan untuk memeriksa guppy hungry.
*/
bool IsGuppyHungry(Guppy g);

/* ======================
Fungsi ResetGuppy
=======================
Fungsi ini digunakan untuk mengatur ulang guppy.
*/
void ResetGuppy(Guppy *g);

#endif
