/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef MIDCIRCLE_H
#define MIDCIRCLE_H

#include "raylib.h"

/* ======================
Fungsi Midcircle
=======================
Fungsi ini digunakan untuk menjalankan proses Midcircle.
*/
void Midcircle(int centerX, int centerY, int radius, Color color);

/* ======================
Fungsi MidcircleFilled
=======================
Fungsi ini digunakan untuk menjalankan proses MidcircleFilled.
*/
void MidcircleFilled(int centerX, int centerY, int radius, Color color);

/* ======================
Fungsi MidcircleThick
=======================
Fungsi ini digunakan untuk menjalankan proses MidcircleThick.
*/
void MidcircleThick(int centerX, int centerY, int radius, int thickness, Color color);

/* ======================
Fungsi MidcircleDashed
=======================
Fungsi ini digunakan untuk menjalankan proses MidcircleDashed.
*/
void MidcircleDashed(int centerX, int centerY, int radius, int dashLen, int gapLen, Color color);

#endif
