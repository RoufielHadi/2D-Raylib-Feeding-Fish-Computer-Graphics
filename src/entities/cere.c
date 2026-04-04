/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "cere.h"

/* ======================
Fungsi InitGuppy
=======================
Fungsi ini digunakan untuk menginisialisasi guppy.
*/
void InitGuppy(Guppy *g, Vector2 pos) {
    g->pos = pos;
    g->vel = (Vector2){58, 0};
    g->wanderTarget = pos;
    g->time = 0;
    g->dir = 1;
    g->tailSwing = 0.0f;
    g->wanderTimer = 0.0f;
    g->finSwing = 0.0f;
    g->scale = 0.55f + (float)GetRandomValue(0, 8) / 100.0f;
    g->depth = 0.70f + (float)GetRandomValue(0, 22) / 100.0f;
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

/* ======================
Fungsi IsGuppyAlive
=======================
Fungsi ini digunakan untuk memeriksa guppy alive.
*/
bool IsGuppyAlive(Guppy g) {
    return g.state != GUPPY_DEAD;
}

/* ======================
Fungsi IsGuppyHungry
=======================
Fungsi ini digunakan untuk memeriksa guppy hungry.
*/
bool IsGuppyHungry(Guppy g) {
    return (g.active && g.state != GUPPY_DEAD && g.hunger > 0.5f);
}

/* ======================
Fungsi ResetGuppy
=======================
Fungsi ini digunakan untuk mengatur ulang guppy.
*/
void ResetGuppy(Guppy *g) {
    g->vel = (Vector2){58, 0};
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
