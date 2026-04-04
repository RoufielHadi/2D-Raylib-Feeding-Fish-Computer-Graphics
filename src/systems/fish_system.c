/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "fish_system.h"
#include "collision.h"
#include <float.h>
#include <math.h>

/* ======================
Fungsi VecLen
=======================
Fungsi ini digunakan untuk menjalankan proses VecLen.
*/
static float VecLen(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

/* ======================
Fungsi VecNorm
=======================
Fungsi ini digunakan untuk menjalankan proses VecNorm.
*/
static Vector2 VecNorm(Vector2 v) {
    float l = VecLen(v);
    if (l <= 0.0001f) return (Vector2){0.0f, 0.0f};
    return (Vector2){v.x / l, v.y / l};
}

/* ======================
Fungsi LerpVec
=======================
Fungsi ini digunakan untuk menginterpolasi vec.
*/
static Vector2 LerpVec(Vector2 a, Vector2 b, float t) {
    return (Vector2){a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}

/* ======================
Fungsi VecSub
=======================
Fungsi ini digunakan untuk menjalankan proses VecSub.
*/
static Vector2 VecSub(Vector2 a, Vector2 b) {
    return (Vector2){a.x - b.x, a.y - b.y};
}

/* ======================
Fungsi VecAdd
=======================
Fungsi ini digunakan untuk menjalankan proses VecAdd.
*/
static Vector2 VecAdd(Vector2 a, Vector2 b) {
    return (Vector2){a.x + b.x, a.y + b.y};
}

/* ======================
Fungsi VecScale
=======================
Fungsi ini digunakan untuk menjalankan proses VecScale.
*/
static Vector2 VecScale(Vector2 v, float scale) {
    return (Vector2){v.x * scale, v.y * scale};
}

/* ======================
Fungsi ClampFloat
=======================
Fungsi ini digunakan untuk membatasi float.
*/
static float ClampFloat(float value, float minValue, float maxValue) {
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

/* ======================
Fungsi RandomFloat
=======================
Fungsi ini digunakan untuk menjalankan proses RandomFloat.
*/
static float RandomFloat(float minValue, float maxValue) {
    return minValue + ((float)GetRandomValue(0, 10000) / 10000.0f) * (maxValue - minValue);
}

/* ======================
Fungsi RandomSwimPoint
=======================
Fungsi ini digunakan untuk menjalankan proses RandomSwimPoint.
*/
static Vector2 RandomSwimPoint(void) {
    float marginX = 90.0f;
    float top = 140.0f;
    float bottom = (float)GetScreenHeight() - 120.0f;
    return (Vector2){
        RandomFloat(marginX, (float)GetScreenWidth() - marginX),
        RandomFloat(top, bottom)
    };
}

/* ======================
Fungsi ChooseNewTarget
=======================
Fungsi ini digunakan untuk menjalankan proses ChooseNewTarget.
*/
static void ChooseNewTarget(Vector2 *target, float *timer, float minDelay, float maxDelay) {
    *target = RandomSwimPoint();
    *timer = RandomFloat(minDelay, maxDelay);
}

/* ======================
Fungsi AvoidWalls
=======================
Fungsi ini digunakan untuk menjalankan proses AvoidWalls.
*/
static void AvoidWalls(Vector2 pos, Vector2 *targetVel, float push) {
    float left = 70.0f;
    float right = (float)GetScreenWidth() - 70.0f;
    float top = 138.0f;
    float bottom = (float)GetScreenHeight() - 110.0f;

    if (pos.x < left) targetVel->x += push;
    if (pos.x > right) targetVel->x -= push;
    if (pos.y < top) targetVel->y += push * 0.7f;
    if (pos.y > bottom) targetVel->y -= push * 0.7f;
}

/* ======================
Fungsi ClampToAquarium
=======================
Fungsi ini digunakan untuk membatasi to aquarium.
*/
static void ClampToAquarium(Vector2 *pos, Vector2 *vel) {
    float minX = 55.0f;
    float maxX = (float)GetScreenWidth() - 55.0f;
    float minY = 128.0f;
    float maxY = (float)GetScreenHeight() - 95.0f;

    if (pos->x < minX) {
        pos->x = minX;
        vel->x = fabsf(vel->x);
    }
    if (pos->x > maxX) {
        pos->x = maxX;
        vel->x = -fabsf(vel->x);
    }
    if (pos->y < minY) {
        pos->y = minY;
        vel->y = fabsf(vel->y);
    }
    if (pos->y > maxY) {
        pos->y = maxY;
        vel->y = -fabsf(vel->y);
    }
}

/* ======================
Fungsi UpdateDirection
=======================
Fungsi ini digunakan untuk memperbarui direction.
*/
static void UpdateDirection(float *dir, Vector2 vel) {
    if (fabsf(vel.x) > 0.5f) {
        *dir = (vel.x >= 0.0f) ? 1.0f : -1.0f;
    }
}

/* ======================
Fungsi UpdateDeathMotion
=======================
Fungsi ini digunakan untuk memperbarui death motion.
*/
static void UpdateDeathMotion(float *flipAngle, Vector2 *pos, float dt) {
    *flipAngle = ClampFloat(*flipAngle + dt * 3.0f, 0.0f, PI);
    pos->y -= 14.0f * dt;
}

/* ======================
Fungsi SeparationForceGuppy
=======================
Fungsi ini digunakan untuk menjalankan proses SeparationForceGuppy.
*/
static Vector2 SeparationForceGuppy(const Guppy *self, const Guppy *guppies, int count) {
    Vector2 force = {0.0f, 0.0f};
    for (int i = 0; i < count; i++) {
        if (&guppies[i] == self || !guppies[i].active) continue;
        Vector2 delta = VecSub(self->pos, guppies[i].pos);
        float dist = VecLen(delta);
        if (dist > 0.001f && dist < 48.0f) {
            force = VecAdd(force, VecScale(VecNorm(delta), (48.0f - dist) * 0.9f));
        }
    }
    return force;
}

/* ======================
Fungsi SeparationForceCarnivore
=======================
Fungsi ini digunakan untuk menjalankan proses SeparationForceCarnivore.
*/
static Vector2 SeparationForceCarnivore(const Carnivore *self, const Carnivore *arr, int count) {
    Vector2 force = {0.0f, 0.0f};
    for (int i = 0; i < count; i++) {
        if (&arr[i] == self || !arr[i].active) continue;
        Vector2 delta = VecSub(self->pos, arr[i].pos);
        float dist = VecLen(delta);
        if (dist > 0.001f && dist < 62.0f) {
            force = VecAdd(force, VecScale(VecNorm(delta), (62.0f - dist) * 0.8f));
        }
    }
    return force;
}

/* ======================
Fungsi SeparationForceUltravore
=======================
Fungsi ini digunakan untuk menjalankan proses SeparationForceUltravore.
*/
static Vector2 SeparationForceUltravore(const Ultravore *self, const Ultravore *arr, int count) {
    Vector2 force = {0.0f, 0.0f};
    for (int i = 0; i < count; i++) {
        if (&arr[i] == self || !arr[i].active) continue;
        Vector2 delta = VecSub(self->pos, arr[i].pos);
        float dist = VecLen(delta);
        if (dist > 0.001f && dist < 72.0f) {
            force = VecAdd(force, VecScale(VecNorm(delta), (72.0f - dist) * 0.75f));
        }
    }
    return force;
}

/* ======================
Fungsi UpdateGuppies
=======================
Fungsi ini digunakan untuk memperbarui guppies.
*/
void UpdateGuppies(Guppy *guppies, int guppyCount, Food *foods, int foodCount, float dt) {
    const float hungerRate = 1.0f;
    const float hungerThreshold = 5.0f;
    const float hungerMax = 10.0f;
    const float feedStart = 2.5f;
    const float speedNormal = 70.0f;
    const float speedHungry = 105.0f;
    const float maxSpeed = 120.0f;
    const float steerLerp = 0.10f;
    const float eatDist = 18.0f;

    // Fase awal mengatur status lapar dan animasi dasar tiap ikan cere.
    for (int i = 0; i < guppyCount; i++) {
        Guppy *g = &guppies[i];
        if (!g->active) continue;

        g->time += dt;
        g->wanderTimer -= dt;
        g->hunger += hungerRate * dt;
        g->tailSwing = sinf(g->time * 11.0f) * 7.0f;
        g->finSwing = sinf(g->time * 9.0f + (float)i) * 4.0f;
        g->eatFlash = ClampFloat(g->eatFlash - dt * 2.8f, 0.0f, 1.0f);

        if (g->state == GUPPY_DEAD) {
            g->tailSwing = 0.0f;
            g->finSwing = 0.0f;
            g->deathTimer += dt;
            UpdateDeathMotion(&g->flipAngle, &g->pos, dt);
            if (g->deathTimer > 2.6f || g->pos.y < 120.0f) {
                g->active = false;
            }
            continue;
        }
        if (g->hunger > hungerMax) {
            g->state = GUPPY_DEAD;
            g->vel = (Vector2){0.0f, 0.0f};
            g->tailSwing = 0.0f;
            g->finSwing = 0.0f;
            g->deathTimer = 0.0f;
            continue;
        }
        g->state = (g->hunger > hungerThreshold) ? GUPPY_HUNGRY : GUPPY_NORMAL;

        float best = FLT_MAX;
        float secondBest = FLT_MAX;
        int bestIdx = -1;
        int secondIdx = -1;
        for (int j = 0; j < foodCount; j++) {
            if (!foods[j].active) continue;
            float d = DistanceBetween(g->pos, foods[j].pos);
            if (d < best) {
                secondBest = best;
                secondIdx = bestIdx;
                best = d;
                bestIdx = j;
            } else if (d < secondBest) {
                secondBest = d;
                secondIdx = j;
            }
        }

        if (g->hunger < feedStart) {
            bestIdx = -1;
            secondIdx = -1;
        }

        if (bestIdx != g->targetIndex) {
            g->targetIndex = bestIdx;
            g->targetTimer = 0.0f;
        } else if (bestIdx >= 0) {
            g->targetTimer += dt;
        }
        if (g->targetTimer > 3.0f && secondIdx >= 0) {
            bestIdx = secondIdx;
            g->targetIndex = secondIdx;
            g->targetTimer = 0.0f;
        }

        Vector2 targetVel = {0.0f, 0.0f};
        if (bestIdx >= 0) {
            Vector2 toFood = VecSub(foods[bestIdx].pos, g->pos);
            float dist = VecLen(toFood);
            Vector2 dir = VecNorm(toFood);
            float speed = (g->state == GUPPY_HUNGRY) ? speedHungry : speedNormal;
            targetVel = VecScale(dir, speed);
            targetVel = VecAdd(targetVel, SeparationForceGuppy(g, guppies, guppyCount));
            AvoidWalls(g->pos, &targetVel, 35.0f);
            g->vel = LerpVec(g->vel, targetVel, steerLerp);

            if (dist < eatDist) {
                MarkFoodEaten(&foods[bestIdx]);
                g->hunger = 0.0f;
                g->state = GUPPY_NORMAL;
                g->eatFlash = 1.0f;
                g->targetIndex = -1;
                g->targetTimer = 0.0f;
                ChooseNewTarget(&g->wanderTarget, &g->wanderTimer, 1.5f, 3.5f);
            }
        } else {
            g->targetIndex = -1;
            g->targetTimer = 0.0f;
            if (g->wanderTimer <= 0.0f || DistanceBetween(g->pos, g->wanderTarget) < 26.0f) {
                ChooseNewTarget(&g->wanderTarget, &g->wanderTimer, 1.8f, 4.0f);
            }
            Vector2 toTarget = VecSub(g->wanderTarget, g->pos);
            Vector2 dir = VecNorm(toTarget);
            float cruise = speedNormal * (0.55f + 0.18f * sinf(g->time * 0.8f + (float)i));
            targetVel = VecScale(dir, cruise);
            targetVel.y += sinf(g->time * 2.0f + (float)i) * 12.0f;
            targetVel = VecAdd(targetVel, SeparationForceGuppy(g, guppies, guppyCount));
            AvoidWalls(g->pos, &targetVel, 40.0f);
            g->vel = LerpVec(g->vel, targetVel, 0.06f);
        }

        float speed = VecLen(g->vel);
        if (speed > maxSpeed) {
            g->vel = VecScale(VecNorm(g->vel), maxSpeed);
        }
        g->pos = VecAdd(g->pos, VecScale(g->vel, dt));
        ClampToAquarium(&g->pos, &g->vel);
        UpdateDirection(&g->dir, g->vel);
    }
}

/* ======================
Fungsi UpdateCarnivores
=======================
Fungsi ini digunakan untuk memperbarui carnivores.
*/
void UpdateCarnivores(Carnivore *carnivores, int carnivoreCount, Guppy *guppies, int guppyCount, float dt) {
    const float hungerRate = 1.0f;
    const float hungerThreshold = 10.0f;
    const float hungerMax = 20.0f;
    const float feedStart = 5.0f;
    const float speedNormal = 80.0f;
    const float speedHungry = 124.0f;
    const float maxSpeed = 150.0f;
    const float steerLerp = 0.12f;
    const float eatDist = 22.0f;

    for (int i = 0; i < carnivoreCount; i++) {
        Carnivore *c = &carnivores[i];
        if (!c->active) continue;

        c->time += dt;
        c->wanderTimer -= dt;
        c->hunger += hungerRate * dt;
        c->tailSwing = sinf(c->time * 10.0f) * 8.5f;
        c->finSwing = sinf(c->time * 7.0f + (float)i) * 4.5f;
        c->eatFlash = ClampFloat(c->eatFlash - dt * 2.5f, 0.0f, 1.0f);

        if (c->state == CARNIVORE_DEAD) {
            c->tailSwing = 0.0f;
            c->finSwing = 0.0f;
            c->deathTimer += dt;
            UpdateDeathMotion(&c->flipAngle, &c->pos, dt);
            if (c->deathTimer > 2.8f || c->pos.y < 120.0f) {
                c->active = false;
            }
            continue;
        }
        if (c->hunger > hungerMax) {
            c->state = CARNIVORE_DEAD;
            c->vel = (Vector2){0.0f, 0.0f};
            c->tailSwing = 0.0f;
            c->finSwing = 0.0f;
            c->deathTimer = 0.0f;
            continue;
        }
        c->state = (c->hunger > hungerThreshold) ? CARNIVORE_HUNGRY : CARNIVORE_NORMAL;

        float best = FLT_MAX;
        int bestIdx = -1;
        for (int j = 0; j < guppyCount; j++) {
            if (!guppies[j].active || guppies[j].state == GUPPY_DEAD) continue;
            float d = DistanceBetween(c->pos, guppies[j].pos);
            if (d < best) {
                best = d;
                bestIdx = j;
            }
        }

        if (c->hunger < feedStart) {
            bestIdx = -1;
        }

        Vector2 targetVel = {0.0f, 0.0f};
        if (bestIdx >= 0) {
            Vector2 toTarget = VecSub(guppies[bestIdx].pos, c->pos);
            float dist = VecLen(toTarget);
            Vector2 dir = VecNorm(toTarget);
            float speed = (c->state == CARNIVORE_HUNGRY) ? speedHungry : speedNormal;
            targetVel = VecScale(dir, speed);
            targetVel = VecAdd(targetVel, SeparationForceCarnivore(c, carnivores, carnivoreCount));
            AvoidWalls(c->pos, &targetVel, 42.0f);
            c->vel = LerpVec(c->vel, targetVel, steerLerp);

            if (dist < eatDist) {
                guppies[bestIdx].active = false;
                guppies[bestIdx].state = GUPPY_DEAD;
                c->hunger = 0.0f;
                c->state = CARNIVORE_NORMAL;
                c->eatFlash = 1.0f;
                ChooseNewTarget(&c->wanderTarget, &c->wanderTimer, 1.8f, 4.5f);
            }
        } else {
            if (c->wanderTimer <= 0.0f || DistanceBetween(c->pos, c->wanderTarget) < 30.0f) {
                ChooseNewTarget(&c->wanderTarget, &c->wanderTimer, 2.0f, 4.8f);
            }
            Vector2 toTarget = VecSub(c->wanderTarget, c->pos);
            Vector2 dir = VecNorm(toTarget);
            float cruise = speedNormal * (0.65f + 0.12f * cosf(c->time * 0.9f + i));
            targetVel = VecScale(dir, cruise);
            targetVel.y += sinf(c->time * 1.6f + (float)i) * 10.0f;
            targetVel = VecAdd(targetVel, SeparationForceCarnivore(c, carnivores, carnivoreCount));
            AvoidWalls(c->pos, &targetVel, 45.0f);
            c->vel = LerpVec(c->vel, targetVel, 0.07f);
        }

        float speed = VecLen(c->vel);
        if (speed > maxSpeed) {
            c->vel = VecScale(VecNorm(c->vel), maxSpeed);
        }
        c->pos = VecAdd(c->pos, VecScale(c->vel, dt));
        ClampToAquarium(&c->pos, &c->vel);
        UpdateDirection(&c->dir, c->vel);
    }
}

/* ======================
Fungsi UpdateUltravoids
=======================
Fungsi ini digunakan untuk memperbarui ultravoids.
*/
void UpdateUltravoids(Ultravore *ultravoids, int ultravoreCount, Carnivore *carnivores, int carnivoreCount, float dt) {
    const float hungerRate = 1.0f;
    const float hungerThreshold = 15.0f;
    const float hungerMax = 30.0f;
    const float feedStart = 7.5f;
    const float speedNormal = 72.0f;
    const float speedHungry = 128.0f;
    const float maxSpeed = 170.0f;
    const float steerLerp = 0.11f;
    const float eatDist = 26.0f;

    for (int i = 0; i < ultravoreCount; i++) {
        Ultravore *u = &ultravoids[i];
        if (!u->active) continue;

        u->time += dt;
        u->wanderTimer -= dt;
        u->hunger += hungerRate * dt;
        u->tailSwing = sinf(u->time * 8.0f) * 10.0f;
        u->finSwing = sinf(u->time * 6.0f + (float)i) * 5.0f;
        u->eatFlash = ClampFloat(u->eatFlash - dt * 2.2f, 0.0f, 1.0f);

        if (u->state == ULTRA_DEAD) {
            u->tailSwing = 0.0f;
            u->finSwing = 0.0f;
            u->deathTimer += dt;
            UpdateDeathMotion(&u->flipAngle, &u->pos, dt);
            if (u->deathTimer > 3.2f || u->pos.y < 120.0f) {
                u->active = false;
            }
            continue;
        }
        if (u->hunger > hungerMax) {
            u->state = ULTRA_DEAD;
            u->vel = (Vector2){0.0f, 0.0f};
            u->tailSwing = 0.0f;
            u->finSwing = 0.0f;
            u->deathTimer = 0.0f;
            continue;
        }
        u->state = (u->hunger > hungerThreshold) ? ULTRA_HUNGRY : ULTRA_NORMAL;

        float best = FLT_MAX;
        int bestIdx = -1;
        for (int j = 0; j < carnivoreCount; j++) {
            if (!carnivores[j].active || carnivores[j].state == CARNIVORE_DEAD) continue;
            float d = DistanceBetween(u->pos, carnivores[j].pos);
            if (d < best) {
                best = d;
                bestIdx = j;
            }
        }

        if (u->hunger < feedStart) {
            bestIdx = -1;
        }

        Vector2 targetVel = {0.0f, 0.0f};
        if (bestIdx >= 0) {
            Vector2 toTarget = VecSub(carnivores[bestIdx].pos, u->pos);
            float dist = VecLen(toTarget);
            Vector2 dir = VecNorm(toTarget);
            float speed = (u->state == ULTRA_HUNGRY) ? speedHungry : speedNormal;
            targetVel = VecScale(dir, speed);
            targetVel = VecAdd(targetVel, SeparationForceUltravore(u, ultravoids, ultravoreCount));
            AvoidWalls(u->pos, &targetVel, 48.0f);
            u->vel = LerpVec(u->vel, targetVel, steerLerp);

            if (dist < eatDist) {
                carnivores[bestIdx].active = false;
                carnivores[bestIdx].state = CARNIVORE_DEAD;
                u->hunger = 0.0f;
                u->state = ULTRA_NORMAL;
                u->eatFlash = 1.0f;
                ChooseNewTarget(&u->wanderTarget, &u->wanderTimer, 2.0f, 5.0f);
            }
        } else {
            if (u->wanderTimer <= 0.0f || DistanceBetween(u->pos, u->wanderTarget) < 34.0f) {
                ChooseNewTarget(&u->wanderTarget, &u->wanderTimer, 2.2f, 5.2f);
            }
            Vector2 toTarget = VecSub(u->wanderTarget, u->pos);
            Vector2 dir = VecNorm(toTarget);
            float cruise = speedNormal * (0.72f + 0.10f * sinf(u->time * 0.7f + i));
            targetVel = VecScale(dir, cruise);
            targetVel.y += sinf(u->time * 1.3f + (float)i) * 8.0f;
            targetVel = VecAdd(targetVel, SeparationForceUltravore(u, ultravoids, ultravoreCount));
            AvoidWalls(u->pos, &targetVel, 52.0f);
            u->vel = LerpVec(u->vel, targetVel, 0.07f);
        }

        float speed = VecLen(u->vel);
        if (speed > maxSpeed) {
            u->vel = VecScale(VecNorm(u->vel), maxSpeed);
        }
        u->pos = VecAdd(u->pos, VecScale(u->vel, dt));
        ClampToAquarium(&u->pos, &u->vel);
        UpdateDirection(&u->dir, u->vel);
    }
}
