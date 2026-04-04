/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef BRESENHAM_H
#define BRESENHAM_H

#include "raylib.h"

/* ======================
Fungsi BresenhamLine
=======================
Fungsi ini digunakan untuk menjalankan proses BresenhamLine.
*/
void BresenhamLine(int x1, int y1, int x2, int y2, Color color);

/* ======================
Fungsi Bres_DashedLine
=======================
Fungsi ini digunakan untuk menjalankan proses Bres_DashedLine.
*/
void Bres_DashedLine(int x1, int y1, int x2, int y2, int dashLen, int gapLen, Color color);

/* ======================
Fungsi Bres_ThickLine
=======================
Fungsi ini digunakan untuk menjalankan proses Bres_ThickLine.
*/
void Bres_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color);

/* ======================
Fungsi Bres_DashDotLine
=======================
Fungsi ini digunakan untuk menjalankan proses Bres_DashDotLine.
*/
void Bres_DashDotLine(int x1, int y1, int x2, int y2, Color color);

#endif
