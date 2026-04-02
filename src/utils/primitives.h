#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "raylib.h"

void Prim_DrawLine(int startX, int startY, int endX, int endY, Color color);
void Prim_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);
void Prim_DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color);
void Prim_DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);
void Prim_DrawCircle(int centerX, int centerY, float radius, Color color);
void Prim_DrawCircleV(Vector2 center, float radius, Color color);
void Prim_DrawCircleLines(int centerX, int centerY, float radius, Color color);
void Prim_DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);
void Prim_DrawRectangle(int posX, int posY, int width, int height, Color color);
void Prim_DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);
void Prim_DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, Color color);
void Prim_DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color);
void Prim_DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom);
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
