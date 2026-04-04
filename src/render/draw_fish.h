/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef DRAW_FISH_H
#define DRAW_FISH_H

#include "raylib.h"
#include "../entities/cere.h"
#include "../entities/lele.h"
#include "../entities/toman.h"

/* ======================
Fungsi DrawGuppy
=======================
Fungsi ini digunakan untuk menggambar guppy.
*/
void DrawGuppy(const Guppy *g, float animTime);

/* ======================
Fungsi DrawPiranha
=======================
Fungsi ini digunakan untuk menggambar piranha.
*/
void DrawPiranha(const Carnivore *c, float animTime);

/* ======================
Fungsi DrawUltravore
=======================
Fungsi ini digunakan untuk menggambar ultravore.
*/
void DrawUltravore(const Ultravore *u, float animTime);

/* ======================
Fungsi DrawGuppyPreview
=======================
Fungsi ini digunakan untuk menggambar guppy preview.
*/
void DrawGuppyPreview(const Guppy *g, float animTime);

/* ======================
Fungsi DrawPiranhaPreview
=======================
Fungsi ini digunakan untuk menggambar piranha preview.
*/
void DrawPiranhaPreview(const Carnivore *c, float animTime);

/* ======================
Fungsi DrawUltravorePreview
=======================
Fungsi ini digunakan untuk menggambar ultravore preview.
*/
void DrawUltravorePreview(const Ultravore *u, float animTime);

/* ======================
Fungsi WarmFishSpriteCaches
=======================
Fungsi ini digunakan untuk menyiapkan fish sprite caches.
*/
void WarmFishSpriteCaches(void);

#endif
