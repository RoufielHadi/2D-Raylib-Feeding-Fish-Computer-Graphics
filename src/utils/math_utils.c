#include "math_utils.h"

#include <math.h>

Vector2 Math_RotatePoint(Vector2 point, Vector2 center, float angleDeg) {
    float rad = angleDeg * DEG2RAD;
    float sinAngle = sinf(rad);
    float cosAngle = cosf(rad);
    float x = point.x - center.x;
    float y = point.y - center.y;

    return (Vector2){
        center.x + x * cosAngle - y * sinAngle,
        center.y + x * sinAngle + y * cosAngle
    };
}
