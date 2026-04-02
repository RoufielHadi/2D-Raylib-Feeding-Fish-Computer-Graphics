#include "game_session.h"

#include "entities/bubble.h"
#include "entities/carnivore.h"
#include "entities/food.h"
#include "entities/guppy.h"
#include "entities/ultravore.h"
#include "systems/fish_system.h"
#include "systems/food_system.h"
#include "ui/button.h"
#include "ui/ui.h"
#include "../render/render.h"

#include "raylib.h"

#include <math.h>

#define MAX_GUPPIES 9
#define MAX_CARNIVORES 3
#define MAX_ULTRAVORES 1
#define MAX_FOOD 40
#define MAX_BUBBLES 24

static float Dist(Vector2 a, Vector2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

static float LerpValue(float a, float b, float t) {
    return a + (b - a) * t;
}

static bool IsPointInsideAquarium(Vector2 point) {
    Rectangle aquariumArea = {
        50.0f,
        108.0f,
        (float)GetScreenWidth() - 100.0f,
        (float)GetScreenHeight() - 200.0f
    };
    return CheckCollisionPointRec(point, aquariumArea);
}

static Vector2 FindSpawnPoint(Vector2 base, float radius,
    Guppy *guppies, int guppyCount,
    Carnivore *carnivores, int carnivoreCount,
    Ultravore *ultravoids, int ultravoreCount) {
    static int s_spawnStep = 0;
    float left = 120.0f;
    float right = (float)GetScreenWidth() - 120.0f;
    float top = 140.0f;
    float bottom = (float)GetScreenHeight() - 170.0f;
    Vector2 slots[8] = {
        {LerpValue(left, right, 0.08f), LerpValue(top, bottom, 0.10f)},
        {LerpValue(left, right, 0.20f), LerpValue(top, bottom, 0.33f)},
        {LerpValue(left, right, 0.34f), LerpValue(top, bottom, 0.58f)},
        {LerpValue(left, right, 0.50f), LerpValue(top, bottom, 0.18f)},
        {LerpValue(left, right, 0.66f), LerpValue(top, bottom, 0.46f)},
        {LerpValue(left, right, 0.80f), LerpValue(top, bottom, 0.16f)},
        {LerpValue(left, right, 0.88f), LerpValue(top, bottom, 0.52f)},
        {LerpValue(left, right, 0.42f), LerpValue(top, bottom, 0.78f)}
    };

    for (int trySlot = 0; trySlot < 8; trySlot++) {
        Vector2 candidate = slots[(s_spawnStep + trySlot) % 8];
        bool overlap = false;

        for (int i = 0; i < guppyCount; i++) {
            if (guppies[i].active && Dist(candidate, guppies[i].pos) < radius) {
                overlap = true;
                break;
            }
        }
        for (int i = 0; i < carnivoreCount && !overlap; i++) {
            if (carnivores[i].active && Dist(candidate, carnivores[i].pos) < radius) {
                overlap = true;
                break;
            }
        }
        for (int i = 0; i < ultravoreCount && !overlap; i++) {
            if (ultravoids[i].active && Dist(candidate, ultravoids[i].pos) < radius) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            s_spawnStep = (s_spawnStep + trySlot + 1) % 8;
            return candidate;
        }
    }

    for (int attempt = 0; attempt < 40; attempt++) {
        Vector2 candidate = {
            base.x + (float)GetRandomValue(-140, 140),
            base.y + (float)GetRandomValue(-80, 80)
        };

        if (candidate.x < left) candidate.x = left;
        if (candidate.x > right) candidate.x = right;
        if (candidate.y < top) candidate.y = top;
        if (candidate.y > bottom) candidate.y = bottom;

        bool overlap = false;
        for (int i = 0; i < guppyCount; i++) {
            if (guppies[i].active && Dist(candidate, guppies[i].pos) < radius) {
                overlap = true;
                break;
            }
        }
        for (int i = 0; i < carnivoreCount && !overlap; i++) {
            if (carnivores[i].active && Dist(candidate, carnivores[i].pos) < radius) {
                overlap = true;
                break;
            }
        }
        for (int i = 0; i < ultravoreCount && !overlap; i++) {
            if (ultravoids[i].active && Dist(candidate, ultravoids[i].pos) < radius) {
                overlap = true;
                break;
            }
        }
        if (!overlap) return candidate;
    }

    return base;
}

static void ResetAquarium(Guppy *guppies, Carnivore *carnivores, Ultravore *ultravoids, Food *foods, Bubble *bubbles) {
    for (int i = 0; i < MAX_GUPPIES; i++) {
        guppies[i].active = false;
        guppies[i].state = GUPPY_NORMAL;
    }
    for (int i = 0; i < MAX_CARNIVORES; i++) {
        carnivores[i].active = false;
        carnivores[i].state = CARNIVORE_NORMAL;
    }
    for (int i = 0; i < MAX_ULTRAVORES; i++) {
        ultravoids[i].active = false;
        ultravoids[i].state = ULTRA_NORMAL;
    }
    for (int i = 0; i < MAX_FOOD; i++) {
        foods[i].active = false;
        foods[i].vel = (Vector2){0.0f, 0.0f};
        foods[i].time = 0.0f;
        foods[i].rotation = 0.0f;
        foods[i].groundedTime = 0.0f;
        foods[i].state = FOOD_EATEN;
    }
    for (int i = 0; i < MAX_BUBBLES; i++) {
        bubbles[i].active = false;
        bubbles[i].state = BUBBLE_POPPED;
    }
}

static void ShutdownGameAudio(bool audioReady, bool hasBgm, Music bgm) {
    if (hasBgm) {
        StopMusicStream(bgm);
        UnloadMusicStream(bgm);
    }
    if (audioReady) {
        CloseAudioDevice();
    }
}

GameSessionResult RunGameSession(Font uiFont, bool hasCustomFont) {
    Guppy guppies[MAX_GUPPIES];
    Carnivore carnivores[MAX_CARNIVORES];
    Ultravore ultravoids[MAX_ULTRAVORES];
    Food foods[MAX_FOOD];
    Bubble bubbles[MAX_BUBBLES];
    float time = 0.0f;
    bool audioReady = false;
    bool hasBgm = false;
    Music bgm = {0};

    UI_SetSharedFont(uiFont, hasCustomFont);
    ResetAquarium(guppies, carnivores, ultravoids, foods, bubbles);
    WarmGameRenderCaches();

    SetAudioStreamBufferSizeDefault(8192);
    InitAudioDevice();
    audioReady = IsAudioDeviceReady();
    if (audioReady && FileExists("assets/sounds/backsound.wav")) {
        bgm = LoadMusicStream("assets/sounds/backsound.wav");
        PlayMusicStream(bgm);
        SetMusicVolume(bgm, UI_GetVolume());
        hasBgm = true;
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        time += dt;
        bool leftClick = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool rightClick = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);

        if (IsKeyPressed(KEY_BACKSPACE)) {
            ShutdownGameAudio(audioReady, hasBgm, bgm);
            return GAME_SESSION_RETURN_HOME;
        }

        if (leftClick || rightClick) {
            Vector2 mp = GetMousePosition();
            UIButtonId bid = leftClick ? UI_HitTest(mp) : UI_BTN_NONE;

            if (bid == UI_BTN_BACK) {
                ShutdownGameAudio(audioReady, hasBgm, bgm);
                return GAME_SESSION_RETURN_HOME;
            } else if (bid == UI_BTN_ADD_GUPPY) {
                for (int i = 0; i < MAX_GUPPIES; i++) {
                    if (!guppies[i].active) {
                        Vector2 spawnBase = {(float)GetScreenWidth() * 0.22f, (float)GetScreenHeight() * 0.50f};
                        Vector2 spawn = FindSpawnPoint(spawnBase, 70.0f,
                            guppies, MAX_GUPPIES, carnivores, MAX_CARNIVORES, ultravoids, MAX_ULTRAVORES);
                        InitGuppy(&guppies[i], spawn);
                        break;
                    }
                }
            } else if (bid == UI_BTN_ADD_CARNIVORE) {
                for (int i = 0; i < MAX_CARNIVORES; i++) {
                    if (!carnivores[i].active) {
                        Vector2 spawnBase = {(float)GetScreenWidth() * 0.50f, (float)GetScreenHeight() * 0.52f};
                        Vector2 spawn = FindSpawnPoint(spawnBase, 88.0f,
                            guppies, MAX_GUPPIES, carnivores, MAX_CARNIVORES, ultravoids, MAX_ULTRAVORES);
                        InitCarnivore(&carnivores[i], spawn);
                        break;
                    }
                }
            } else if (bid == UI_BTN_ADD_ULTRAVORE) {
                for (int i = 0; i < MAX_ULTRAVORES; i++) {
                    if (!ultravoids[i].active) {
                        Vector2 spawnBase = {(float)GetScreenWidth() * 0.74f, (float)GetScreenHeight() * 0.50f};
                        Vector2 spawn = FindSpawnPoint(spawnBase, 96.0f,
                            guppies, MAX_GUPPIES, carnivores, MAX_CARNIVORES, ultravoids, MAX_ULTRAVORES);
                        InitUltravore(&ultravoids[i], spawn);
                        break;
                    }
                }
            } else if (bid == UI_BTN_RESET) {
                ResetAquarium(guppies, carnivores, ultravoids, foods, bubbles);
            } else if (bid == UI_BTN_VOLUME) {
                UI_ToggleVolume();
            } else if (bid == UI_BTN_NONE && IsPointInsideAquarium(mp)) {
                for (int i = 0; i < MAX_FOOD; i++) {
                    if (!foods[i].active) {
                        InitFood(&foods[i], mp);
                        foods[i].pos = mp;
                        foods[i].vel = (Vector2){0.0f, 0.0f};
                        break;
                    }
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && UI_IsVolumeOpen()) {
            UI_HandleVolumeClick(GetMousePosition());
        }

        if (GetRandomValue(0, 100) < 3) {
            for (int i = 0; i < MAX_BUBBLES; i++) {
                if (!bubbles[i].active) {
                    InitBubble(&bubbles[i], (Vector2){
                        (float)GetRandomValue(56, GetScreenWidth() - 56),
                        (float)GetScreenHeight() - 140.0f
                    });
                    break;
                }
            }
        }

        UpdateFoodSystem(foods, MAX_FOOD, dt);
        UpdateGuppies(guppies, MAX_GUPPIES, foods, MAX_FOOD, dt);
        UpdateCarnivores(carnivores, MAX_CARNIVORES, guppies, MAX_GUPPIES, dt);
        UpdateUltravoids(ultravoids, MAX_ULTRAVORES, carnivores, MAX_CARNIVORES, dt);

        for (int i = 0; i < MAX_BUBBLES; i++) {
            Bubble *bubble = &bubbles[i];
            if (!bubble->active) continue;

            bubble->time += dt;
            bubble->pos.y += bubble->vel.y * dt;
            bubble->pos.x += sinf(bubble->time * 2.0f + (float)i) * 8.0f * dt;
            bubble->alpha -= 20.0f * dt;
            if (bubble->alpha < 30.0f) bubble->alpha = 30.0f;
            if (bubble->pos.y + bubble->radius < -10.0f) {
                bubble->active = false;
                bubble->state = BUBBLE_POPPED;
            }
        }

        if (hasBgm) {
            UpdateMusicStream(bgm);
            SetMusicVolume(bgm, UI_GetVolume());
        }

        BeginDrawing();
        ClearBackground(BLACK);
        RenderAll(guppies, MAX_GUPPIES,
            carnivores, MAX_CARNIVORES,
            ultravoids, MAX_ULTRAVORES,
            foods, MAX_FOOD,
            bubbles, MAX_BUBBLES,
            time);
        EndDrawing();
    }

    ShutdownGameAudio(audioReady, hasBgm, bgm);

    return GAME_SESSION_EXIT_APP;
}
