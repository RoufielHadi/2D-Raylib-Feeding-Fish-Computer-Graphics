/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "collision.h"
#include <math.h>

/* ======================
Fungsi DistanceBetween
=======================
Fungsi ini digunakan untuk menjalankan proses DistanceBetween.
*/
float DistanceBetween(Vector2 a, Vector2 b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrtf(dx*dx + dy*dy);
}

/* ======================
Fungsi WithinDistance
=======================
Fungsi ini digunakan untuk menjalankan proses WithinDistance.
*/
bool WithinDistance(Vector2 a, Vector2 b, float thresh) {
	return DistanceBetween(a,b) <= thresh;
}
