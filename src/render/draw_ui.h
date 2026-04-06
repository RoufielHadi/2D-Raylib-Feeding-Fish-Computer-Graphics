/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef DRAW_UI_H
#define DRAW_UI_H

#include "raylib.h"
#include "../entities/cere.h"
#include "../entities/lele.h"
#include "../entities/toman.h"

typedef enum {
    BUBBLE_CTRL_CALM = 0,
    BUBBLE_CTRL_NORMAL,
    BUBBLE_CTRL_TURBO
} BubbleControlMode;

typedef struct {
    bool autoSpeedEnabled;
    bool wireframeEnabled;
    BubbleControlMode bubbleMode;
    float simSpeed;
} AquariumUiOptions;

/* ======================
Fungsi DrawUI
=======================
Fungsi ini digunakan untuk menggambar ui.
*/
void DrawUI(const Guppy *g, const Carnivore *c, const Ultravore *u, const AquariumUiOptions *options);

/* ======================
Fungsi WarmHeaderUICache
=======================
Fungsi ini digunakan untuk menyiapkan header uicache.
*/
void WarmHeaderUICache(void);

#endif
