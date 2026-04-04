/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef DDA_H
#define DDA_H

#include "raylib.h"

/* ======================
Fungsi DDALine
=======================
Fungsi ini digunakan untuk menjalankan proses DDALine.
*/
void DDALine(int x1, int y1, int x2, int y2, Color color);

/* ======================
Fungsi DDA_DashedLine
=======================
Fungsi ini digunakan untuk menjalankan proses DDA_DashedLine.
*/
void DDA_DashedLine(int x1, int y1, int x2, int y2, int dashLen, int gapLen, Color color);

/* ======================
Fungsi DDA_ThickLine
=======================
Fungsi ini digunakan untuk menjalankan proses DDA_ThickLine.
*/
void DDA_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color);

/* ======================
Fungsi DDA_DashDotLine
=======================
Fungsi ini digunakan untuk menjalankan proses DDA_DashDotLine.
*/
void DDA_DashDotLine(int x1, int y1, int x2, int y2, Color color);

#endif
