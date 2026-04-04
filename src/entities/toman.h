/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef TOMAN_H
#define TOMAN_H

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

/* ======================
Fungsi InitUltravore
=======================
Fungsi ini digunakan untuk menginisialisasi ultravore.
*/
void InitUltravore(Ultravore *u, Vector2 pos);

/* ======================
Fungsi IsUltravoreAlive
=======================
Fungsi ini digunakan untuk memeriksa ultravore alive.
*/
bool IsUltravoreAlive(Ultravore u);

/* ======================
Fungsi IsUltravoreHungry
=======================
Fungsi ini digunakan untuk memeriksa ultravore hungry.
*/
bool IsUltravoreHungry(Ultravore u);

/* ======================
Fungsi ResetUltravore
=======================
Fungsi ini digunakan untuk mengatur ulang ultravore.
*/
void ResetUltravore(Ultravore *u);

#endif
