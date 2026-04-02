#include "collision.h"
#include <math.h>

float DistanceBetween(Vector2 a, Vector2 b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrtf(dx*dx + dy*dy);
}

bool WithinDistance(Vector2 a, Vector2 b, float thresh) {
	return DistanceBetween(a,b) <= thresh;
}
