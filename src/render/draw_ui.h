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

/* ======================
Fungsi DrawUI
=======================
Fungsi ini digunakan untuk menggambar ui.
*/
void DrawUI(const Guppy *g, const Carnivore *c, const Ultravore *u);

/* ======================
Fungsi WarmHeaderUICache
=======================
Fungsi ini digunakan untuk menyiapkan header uicache.
*/
void WarmHeaderUICache(void);

#endif
