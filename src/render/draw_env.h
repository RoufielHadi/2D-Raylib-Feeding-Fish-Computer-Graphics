/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef DRAW_ENV_H
#define DRAW_ENV_H

#include "raylib.h"

/* ======================
Fungsi DrawWaterBackground
=======================
Fungsi ini digunakan untuk menggambar water background.
*/
void DrawWaterBackground(void);

/* ======================
Fungsi DrawWaterBackgroundRect
=======================
Fungsi ini digunakan untuk menggambar water background rect.
*/
void DrawWaterBackgroundRect(Rectangle area);

/* ======================
Fungsi DrawSand
=======================
Fungsi ini digunakan untuk menggambar sand.
*/
void DrawSand(void);

/* ======================
Fungsi DrawSeaweed
=======================
Fungsi ini digunakan untuk menggambar seaweed.
*/
void DrawSeaweed(Vector2 pos, float time);

/* ======================
Fungsi DrawCoral
=======================
Fungsi ini digunakan untuk menggambar coral.
*/
void DrawCoral(Vector2 pos);

/* ======================
Fungsi DrawHelmet
=======================
Fungsi ini digunakan untuk menggambar helmet.
*/
void DrawHelmet(Vector2 pos);

/* ======================
Fungsi WarmSeaweedSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan seaweed sprite cache.
*/
void WarmSeaweedSpriteCache(void);

#endif
