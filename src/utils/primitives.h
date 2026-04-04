/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "raylib.h"

/* ======================
Fungsi Prim_DrawLine
=======================
Fungsi ini digunakan untuk menggambar line.
*/
void Prim_DrawLine(int startX, int startY, int endX, int endY, Color color);

/* ======================
Fungsi Prim_DrawLineEx
=======================
Fungsi ini digunakan untuk menggambar line ex.
*/
void Prim_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);

/* ======================
Fungsi Prim_DrawLineBezier
=======================
Fungsi ini digunakan untuk menggambar line bezier.
*/
void Prim_DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color);

/* ======================
Fungsi Prim_DrawTriangle
=======================
Fungsi ini digunakan untuk menggambar triangle.
*/
void Prim_DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);

/* ======================
Fungsi Prim_DrawCircle
=======================
Fungsi ini digunakan untuk menggambar circle.
*/
void Prim_DrawCircle(int centerX, int centerY, float radius, Color color);

/* ======================
Fungsi Prim_DrawCircleV
=======================
Fungsi ini digunakan untuk menggambar circle v.
*/
void Prim_DrawCircleV(Vector2 center, float radius, Color color);

/* ======================
Fungsi Prim_DrawCircleLines
=======================
Fungsi ini digunakan untuk menggambar circle lines.
*/
void Prim_DrawCircleLines(int centerX, int centerY, float radius, Color color);

/* ======================
Fungsi Prim_DrawEllipse
=======================
Fungsi ini digunakan untuk menggambar ellipse.
*/
void Prim_DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);

/* ======================
Fungsi Prim_DrawRectangle
=======================
Fungsi ini digunakan untuk menggambar rectangle.
*/
void Prim_DrawRectangle(int posX, int posY, int width, int height, Color color);

/* ======================
Fungsi Prim_DrawRectangleRounded
=======================
Fungsi ini digunakan untuk menggambar rectangle rounded.
*/
void Prim_DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);

/* ======================
Fungsi Prim_DrawRectangleRoundedLines
=======================
Fungsi ini digunakan untuk menggambar rectangle rounded lines.
*/
void Prim_DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, Color color);

/* ======================
Fungsi Prim_DrawRectangleRoundedLinesEx
=======================
Fungsi ini digunakan untuk menggambar rectangle rounded lines ex.
*/
void Prim_DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color);

/* ======================
Fungsi Prim_DrawRectangleGradientV
=======================
Fungsi ini digunakan untuk menggambar rectangle gradient v.
*/
void Prim_DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);

/* ======================
Fungsi Prim_DrawRing
=======================
Fungsi ini digunakan untuk menggambar ring.
*/
void Prim_DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color);

#ifndef PRIMITIVES_NO_RAYLIB_ALIASES
#define DrawLine Prim_DrawLine
#define DrawLineEx Prim_DrawLineEx
#define DrawLineBezier Prim_DrawLineBezier
#define DrawTriangle Prim_DrawTriangle
#define DrawCircle Prim_DrawCircle
#define DrawCircleV Prim_DrawCircleV
#define DrawCircleLines Prim_DrawCircleLines
#define DrawEllipse Prim_DrawEllipse
#define DrawRectangle Prim_DrawRectangle
#define DrawRectangleRounded Prim_DrawRectangleRounded
#define DrawRectangleRoundedLines Prim_DrawRectangleRoundedLines
#define DrawRectangleRoundedLinesEx Prim_DrawRectangleRoundedLinesEx
#define DrawRectangleGradientV Prim_DrawRectangleGradientV
#define DrawRing Prim_DrawRing
#endif

#endif
