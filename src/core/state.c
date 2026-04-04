/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "raylib.h"

/* ======================
Fungsi Game_GetSeed
=======================
Fungsi ini digunakan untuk mengambil seed.
*/
int Game_GetSeed(void) {
	return (int) (GetTime() * 1000.0);
}
