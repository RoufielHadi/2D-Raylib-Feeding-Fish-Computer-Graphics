#include "draw_fish.h"
#include "../utils/primitives.h"

#include <math.h>

#define FISH_CACHE_FRAMES 16
#define GUPPY_SPRITE_W 176
#define GUPPY_SPRITE_H 112
#define CARNIVORE_SPRITE_W 192
#define CARNIVORE_SPRITE_H 124
#define ULTRAVORE_SPRITE_W 272
#define ULTRAVORE_SPRITE_H 168

static RenderTexture2D s_guppySprites[2][2][FISH_CACHE_FRAMES] = {0};
static unsigned char s_guppySpriteReady[2][2][FISH_CACHE_FRAMES] = {0};
static RenderTexture2D s_piranhaSprites[2][2][FISH_CACHE_FRAMES] = {0};
static unsigned char s_piranhaSpriteReady[2][2][FISH_CACHE_FRAMES] = {0};
static RenderTexture2D s_ultravoreSprites[2][2][FISH_CACHE_FRAMES] = {0};
static unsigned char s_ultravoreSpriteReady[2][2][FISH_CACHE_FRAMES] = {0};

static void DrawGuppyDirect(const Guppy *g, float animTime);
static void DrawPiranhaDirect(const Carnivore *c, float animTime);
static void DrawUltravoreDirect(const Ultravore *u, float animTime);

typedef struct {
    float bodyHalfW;
    float bodyHalfH;
    float tailLength;
    float tailHeight;
    float headLength;
    float mouthLength;
    float eyeOffsetX;
    float eyeOffsetY;
    float ventralBaseX;
    float ventralBaseY;
    float tailRootX;
    float tailRootY;
} FishRigLayout;

typedef struct {
    Vector2 center;
    Vector2 nose;
    Vector2 bodyTop;
    Vector2 tailRootTop;
    Vector2 tailRootBottom;
    Vector2 tailForkTop;
    Vector2 tailForkBottom;
    Vector2 tailCenter;
    Vector2 eye;
    Vector2 ventralBase;
    Vector2 ventralTip;
    Vector2 jawTop;
    Vector2 jawBottom;
    Vector2 mouthTipTop;
    Vector2 mouthTipBottom;
} FishRig;

static float FacingX(float baseX, float offset, float dir)
{
    return baseX + offset * ((dir >= 0.0f) ? 1.0f : -1.0f);
}

static Vector2 FacingPointScaled(float baseX, float baseY, float offsetX, float offsetY, float dir, float rot, float scale)
{
    float facing = (dir >= 0.0f) ? 1.0f : -1.0f;
    return (Vector2){
        baseX + offsetX * scale * facing,
        baseY + offsetY * scale * rot
    };
}

static Color FadeToGray(Color c, float amount)
{
    Color out = c;
    unsigned char gray = (unsigned char)(0.299f * c.r + 0.587f * c.g + 0.114f * c.b);
    out.r = (unsigned char)(c.r + (gray - c.r) * amount);
    out.g = (unsigned char)(c.g + (gray - c.g) * amount);
    out.b = (unsigned char)(c.b + (gray - c.b) * amount);
    return out;
}

static Color ApplyStateTint(Color base, int isHungry, int isDead)
{
    if (isDead) return FadeToGray((Color){245, 245, 245, base.a}, 0.9f);
    if (isHungry) {
        Color green = (Color){110, 210, 120, base.a};
        return ColorAlphaBlend(base, green, (Color){255, 255, 255, 110});
    }
    return base;
}

static Color DeadFinColor(Color base)
{
    return FadeToGray(base, 0.75f);
}

static void DrawSafeTriangle(Vector2 a, Vector2 b, Vector2 c, float dir, Color color)
{
    if (dir < 0.0f) {
        DrawTriangle(a, c, b, color);
    } else {
        DrawTriangle(a, b, c, color);
    }
}

static void DrawDeadEye(float cx, float cy, float dir, float rot, float scale, float offsetX, float offsetY)
{
    float size = 5.5f * scale;
    Vector2 a = FacingPointScaled(cx, cy, offsetX - size, offsetY - size, dir, rot, 1.0f);
    Vector2 b = FacingPointScaled(cx, cy, offsetX + size, offsetY + size, dir, rot, 1.0f);
    Vector2 c = FacingPointScaled(cx, cy, offsetX - size, offsetY + size, dir, rot, 1.0f);
    Vector2 d = FacingPointScaled(cx, cy, offsetX + size, offsetY - size, dir, rot, 1.0f);
    DrawLineEx(a, b, 2.0f * scale, BLACK);
    DrawLineEx(c, d, 2.0f * scale, BLACK);
}

static FishRig BuildFishRig(float cx, float cy, float dir, float rot, float scale,
                            FishRigLayout layout, float bodySway, float tailSwing, float bellyFinSwing, float mouthOpen)
{
    FishRig rig = {0};
    float tailLift = tailSwing * 0.34f;

    rig.center = (Vector2){cx, cy};
    rig.nose = FacingPointScaled(cx, cy, layout.headLength + layout.mouthLength * 0.45f, bodySway * 0.15f, dir, rot, scale);
    rig.bodyTop = FacingPointScaled(cx, cy, -layout.bodyHalfW * 0.22f, -layout.bodyHalfH + bodySway * 0.35f, dir, rot, scale);
    rig.tailRootTop = FacingPointScaled(cx, cy, layout.tailRootX, -layout.tailRootY + tailLift, dir, rot, scale);
    rig.tailRootBottom = FacingPointScaled(cx, cy, layout.tailRootX, layout.tailRootY + tailLift, dir, rot, scale);
    rig.tailForkTop = FacingPointScaled(cx, cy, layout.tailRootX - layout.tailLength, -layout.tailHeight - tailSwing, dir, rot, scale);
    rig.tailForkBottom = FacingPointScaled(cx, cy, layout.tailRootX - layout.tailLength, layout.tailHeight + tailSwing, dir, rot, scale);
    rig.tailCenter = FacingPointScaled(cx, cy, layout.tailRootX - layout.tailLength * 0.68f, tailLift * 0.55f, dir, rot, scale);
    rig.eye = FacingPointScaled(cx, cy, layout.eyeOffsetX, layout.eyeOffsetY + bodySway * 0.08f, dir, rot, scale);
    rig.ventralBase = FacingPointScaled(cx, cy, layout.ventralBaseX, layout.ventralBaseY + bodySway * 0.22f, dir, rot, scale);
    rig.ventralTip = FacingPointScaled(cx, cy, layout.ventralBaseX + 4.0f, layout.ventralBaseY + 10.0f + bellyFinSwing, dir, rot, scale);
    rig.jawTop = FacingPointScaled(cx, cy, layout.headLength - 2.0f, -4.0f - mouthOpen * 0.70f, dir, rot, scale);
    rig.jawBottom = FacingPointScaled(cx, cy, layout.headLength - 1.0f, 5.0f + mouthOpen * 0.85f, dir, rot, scale);
    rig.mouthTipTop = FacingPointScaled(cx, cy, layout.headLength + layout.mouthLength, -1.5f - mouthOpen, dir, rot, scale);
    rig.mouthTipBottom = FacingPointScaled(cx, cy, layout.headLength + layout.mouthLength, 2.0f + mouthOpen * 1.12f, dir, rot, scale);
    return rig;
}

static void DrawTailFan(const FishRig *rig, float dir, Color mainColor, Color accentColor, float scale)
{
    DrawSafeTriangle(rig->tailRootTop, rig->tailForkTop, rig->tailCenter, dir, mainColor);
    DrawSafeTriangle(rig->tailRootBottom, rig->tailCenter, rig->tailForkBottom, dir, accentColor);
    DrawSafeTriangle(rig->tailRootTop, rig->tailCenter, rig->tailRootBottom, dir, ColorAlpha(mainColor, 0.60f));
    DrawLineEx(rig->tailRootTop, rig->tailForkTop, 1.1f * scale, ColorAlpha(BLACK, 0.24f));
    DrawLineEx(rig->tailRootBottom, rig->tailForkBottom, 1.1f * scale, ColorAlpha(BLACK, 0.24f));
    DrawLineEx(rig->tailCenter, rig->tailForkTop, 1.0f * scale, ColorAlpha(WHITE, 0.14f));
    DrawLineEx(rig->tailCenter, rig->tailForkBottom, 1.0f * scale, ColorAlpha(WHITE, 0.14f));
}

static void DrawStaticDorsalFin(Vector2 baseLeft, Vector2 tip, Vector2 baseRight, float dir, Color color, float scale)
{
    DrawSafeTriangle(baseLeft, tip, baseRight, dir, ColorAlpha(color, 0.86f));
    DrawLineEx(baseLeft, tip, 1.0f * scale, ColorAlpha(BLACK, 0.20f));
    DrawLineEx(tip, baseRight, 1.0f * scale, ColorAlpha(BLACK, 0.20f));
}

static void DrawSwingVentralFin(const FishRig *rig, float dir, Color color, float scale)
{
    Vector2 finBack = {rig->ventralBase.x - 6.0f * scale * ((dir >= 0.0f) ? 1.0f : -1.0f), rig->ventralBase.y + 2.0f * scale};
    DrawSafeTriangle(rig->ventralBase, rig->ventralTip, finBack, dir, ColorAlpha(color, 0.80f));
    DrawLineEx(rig->ventralBase, rig->ventralTip, 1.0f * scale, ColorAlpha(BLACK, 0.22f));
}

static void DrawFishMouth(const FishRig *rig, float dir, float scale, Color upperColor, Color lowerColor, Color innerColor, int teethCount)
{
    DrawSafeTriangle(rig->jawTop, rig->mouthTipTop, rig->nose, dir, upperColor);
    DrawSafeTriangle(rig->nose, rig->mouthTipBottom, rig->jawBottom, dir, lowerColor);
    DrawSafeTriangle(rig->jawTop, rig->nose, rig->jawBottom, dir, ColorAlpha(innerColor, 0.68f));

    for (int i = 0; i < teethCount; i++) {
        float t = (float)(i + 0.5f) / (float)teethCount;
        Vector2 topBase = {
            rig->jawTop.x + (rig->mouthTipTop.x - rig->jawTop.x) * t,
            rig->jawTop.y + (rig->mouthTipTop.y - rig->jawTop.y) * t
        };
        Vector2 bottomBase = {
            rig->jawBottom.x + (rig->mouthTipBottom.x - rig->jawBottom.x) * t,
            rig->jawBottom.y + (rig->mouthTipBottom.y - rig->jawBottom.y) * t
        };
        float toothWidth = 1.4f * scale;
        float toothHeight = 3.0f * scale;
        Vector2 topLeft = {topBase.x - toothWidth, topBase.y};
        Vector2 topRight = {topBase.x + toothWidth, topBase.y};
        Vector2 topTip = {topBase.x, topBase.y + toothHeight};
        Vector2 bottomLeft = {bottomBase.x - toothWidth, bottomBase.y};
        Vector2 bottomRight = {bottomBase.x + toothWidth, bottomBase.y};
        Vector2 bottomTip = {bottomBase.x, bottomBase.y - toothHeight};
        DrawSafeTriangle(topLeft, topTip, topRight, dir, (Color){238, 236, 228, 255});
        DrawSafeTriangle(bottomLeft, bottomTip, bottomRight, dir, (Color){238, 236, 228, 255});
    }
}

static void DrawFishEye(Vector2 eye, float dir, float scale, Color iris, bool dead)
{
    if (dead) {
        DrawCircleV(eye, 8.0f * scale, LIGHTGRAY);
        return;
    }

    DrawCircleV(eye, 8.0f * scale, iris);
    DrawCircleV((Vector2){eye.x + 2.4f * dir * scale, eye.y}, 4.0f * scale, BLACK);
    DrawCircleV((Vector2){eye.x + 1.6f * dir * scale, eye.y - 2.2f * scale}, 1.6f * scale, WHITE);
}

static float ComputeMouthOpen(float eatFlash, float animTime, float phase, float scaleFactor)
{
    if (eatFlash <= 0.01f) return 0.0f;
    float chew = 0.5f + 0.5f * sinf(animTime * 22.0f + phase);
    return (2.0f + chew * 7.0f) * eatFlash * scaleFactor;
}

static int WrapFrameIndex(int frame)
{
    int wrapped = frame % FISH_CACHE_FRAMES;
    return (wrapped < 0) ? (wrapped + FISH_CACHE_FRAMES) : wrapped;
}

static int QuantizePhaseFrame(float phase)
{
    float normalized = phase / (2.0f * PI);
    normalized -= floorf(normalized);
    return WrapFrameIndex((int)floorf(normalized * FISH_CACHE_FRAMES));
}

static void DrawCachedFishSprite(RenderTexture2D texture, Vector2 pos, float scale)
{
    float texW = (float)texture.texture.width;
    float texH = (float)texture.texture.height;
    Rectangle src = {0.0f, 0.0f, texW, -texH};
    Rectangle dst = {
        pos.x - texW * scale * 0.5f,
        pos.y - texH * scale * 0.5f,
        texW * scale,
        texH * scale
    };
    DrawTexturePro(texture.texture, src, dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
}

static void EnsureGuppySprite(int hungryIndex, int dirIndex, int frame)
{
    int wrappedFrame = WrapFrameIndex(frame);
    if (s_guppySpriteReady[hungryIndex][dirIndex][wrappedFrame]) return;

    s_guppySprites[hungryIndex][dirIndex][wrappedFrame] = LoadRenderTexture(GUPPY_SPRITE_W, GUPPY_SPRITE_H);
    BeginTextureMode(s_guppySprites[hungryIndex][dirIndex][wrappedFrame]);
    ClearBackground(BLANK);

    float phase = ((float)wrappedFrame / (float)FISH_CACHE_FRAMES) * 2.0f * PI;
    Guppy guppy = {0};
    guppy.active = true;
    guppy.state = hungryIndex ? GUPPY_HUNGRY : GUPPY_NORMAL;
    guppy.scale = 1.0f;
    guppy.dir = dirIndex ? 1.0f : -1.0f;
    guppy.pos = (Vector2){GUPPY_SPRITE_W * 0.54f, GUPPY_SPRITE_H * 0.52f};
    guppy.tailSwing = sinf(phase * 1.6f) * 7.0f;
    guppy.finSwing = sinf(phase * 1.8f) * 4.0f;
    DrawGuppyDirect(&guppy, phase);

    EndTextureMode();
    s_guppySpriteReady[hungryIndex][dirIndex][wrappedFrame] = 1;
}

static void EnsurePiranhaSprite(int hungryIndex, int dirIndex, int frame)
{
    int wrappedFrame = WrapFrameIndex(frame);
    if (s_piranhaSpriteReady[hungryIndex][dirIndex][wrappedFrame]) return;

    s_piranhaSprites[hungryIndex][dirIndex][wrappedFrame] = LoadRenderTexture(CARNIVORE_SPRITE_W, CARNIVORE_SPRITE_H);
    BeginTextureMode(s_piranhaSprites[hungryIndex][dirIndex][wrappedFrame]);
    ClearBackground(BLANK);

    float phase = ((float)wrappedFrame / (float)FISH_CACHE_FRAMES) * 2.0f * PI;
    Carnivore carnivore = {0};
    carnivore.active = true;
    carnivore.state = hungryIndex ? CARNIVORE_HUNGRY : CARNIVORE_NORMAL;
    carnivore.scale = 1.0f;
    carnivore.dir = dirIndex ? 1.0f : -1.0f;
    carnivore.pos = (Vector2){CARNIVORE_SPRITE_W * 0.56f, CARNIVORE_SPRITE_H * 0.52f};
    carnivore.tailSwing = sinf(phase * 1.5f) * 8.5f;
    carnivore.finSwing = sinf(phase * 1.2f) * 4.5f;
    DrawPiranhaDirect(&carnivore, phase);

    EndTextureMode();
    s_piranhaSpriteReady[hungryIndex][dirIndex][wrappedFrame] = 1;
}

static void EnsureUltravoreSprite(int hungryIndex, int dirIndex, int frame)
{
    int wrappedFrame = WrapFrameIndex(frame);
    if (s_ultravoreSpriteReady[hungryIndex][dirIndex][wrappedFrame]) return;

    s_ultravoreSprites[hungryIndex][dirIndex][wrappedFrame] = LoadRenderTexture(ULTRAVORE_SPRITE_W, ULTRAVORE_SPRITE_H);
    BeginTextureMode(s_ultravoreSprites[hungryIndex][dirIndex][wrappedFrame]);
    ClearBackground(BLANK);

    float phase = ((float)wrappedFrame / (float)FISH_CACHE_FRAMES) * 2.0f * PI;
    Ultravore ultravore = {0};
    ultravore.active = true;
    ultravore.state = hungryIndex ? ULTRA_HUNGRY : ULTRA_NORMAL;
    ultravore.scale = 1.0f;
    ultravore.dir = dirIndex ? 1.0f : -1.0f;
    ultravore.pos = (Vector2){ULTRAVORE_SPRITE_W * 0.57f, ULTRAVORE_SPRITE_H * 0.53f};
    ultravore.tailSwing = sinf(phase * 1.1f) * 10.0f;
    ultravore.finSwing = sinf(phase * 0.9f) * 5.0f;
    DrawUltravoreDirect(&ultravore, phase);

    EndTextureMode();
    s_ultravoreSpriteReady[hungryIndex][dirIndex][wrappedFrame] = 1;
}

static void DrawGuppyBody(const FishRig *rig, float cx, float cy, float dir, float rot, float scale, Color body, Color belly)
{
    DrawEllipse((int)FacingX(cx, -3.0f * scale, dir), (int)cy, (int)(40.0f * scale), (int)(22.0f * scale * fabsf(rot)), body);
    DrawEllipse((int)FacingX(cx, 6.0f * scale, dir), (int)(cy + 6.0f * rot * scale), (int)(23.0f * scale), (int)(9.0f * scale), belly);
    DrawEllipse((int)FacingX(cx, -1.0f * scale, dir), (int)(cy - 8.0f * scale), (int)(25.0f * scale), (int)(6.0f * scale), ColorAlpha(WHITE, 0.18f));
    DrawLineBezier(
        FacingPointScaled(cx, cy, -10.0f, 6.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 16.0f, 7.0f, dir, rot, scale),
        1.0f * scale, ColorAlpha((Color){140, 90, 50, 255}, 0.18f));
}

static void DrawCarnivoreBody(const FishRig *rig, float cx, float cy, float dir, float rot, float scale,
                              Color upper, Color belly, Color stripe)
{
    DrawEllipse((int)FacingX(cx, -5.0f * scale, dir), (int)cy, (int)(46.0f * scale), (int)(29.0f * scale * fabsf(rot)), upper);
    DrawEllipse((int)FacingX(cx, -1.0f * scale, dir), (int)(cy + 12.0f * rot * scale), (int)(34.0f * scale), (int)(12.0f * scale), belly);
    DrawEllipse((int)FacingX(cx, -8.0f * scale, dir), (int)(cy - 11.0f * scale), (int)(24.0f * scale), (int)(8.0f * scale), ColorAlpha(WHITE, 0.07f));

    Vector2 stripePoints[9] = {
        FacingPointScaled(cx, cy, 15.0f, -1.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 6.0f, 8.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -1.0f, 1.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -9.0f, 9.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -18.0f, 2.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -26.0f, 10.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -33.0f, 3.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -40.0f, 8.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, -45.0f, 2.0f, dir, rot, scale)
    };
    for (int i = 0; i < 8; i++) {
        DrawLineEx(stripePoints[i], stripePoints[i + 1], 2.2f * scale, ColorAlpha(stripe, 0.78f));
    }

    DrawLineBezier(rig->bodyTop, rig->tailRootTop, 1.2f * scale, ColorAlpha(BLACK, 0.16f));
}

static void DrawUltravoreBody(const FishRig *rig, float cx, float cy, float dir, float rot, float scale,
                              Color body, Color backShade)
{
    DrawEllipse((int)FacingX(cx, -14.0f * scale, dir), (int)cy, (int)(68.0f * scale), (int)(35.0f * scale * fabsf(rot)), body);
    DrawEllipse((int)FacingX(cx, -24.0f * scale, dir), (int)(cy - 5.0f * rot * scale), (int)(48.0f * scale), (int)(25.0f * scale), ColorAlpha(backShade, 0.80f));
    DrawEllipse((int)FacingX(cx, -20.0f * scale, dir), (int)(cy - 13.0f * scale), (int)(38.0f * scale), (int)(7.0f * scale), ColorAlpha(WHITE, 0.08f));
    DrawLineBezier(rig->bodyTop, rig->tailRootTop, 1.4f * scale, ColorAlpha(BLACK, 0.18f));
    DrawLineBezier(
        FacingPointScaled(cx, cy, -38.0f, 12.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 4.0f, 18.0f, dir, rot, scale),
        1.1f * scale, ColorAlpha(BLACK, 0.18f));
}

static void DrawGuppyDirect(const Guppy *g, float animTime)
{
    if (!g || !g->active) return;

    float dir = (g->dir >= 0.0f) ? 1.0f : -1.0f;
    float rot = (g->state == GUPPY_DEAD) ? -1.0f : 1.0f;
    float scale = g->scale;
    float bodyWave = (g->state == GUPPY_DEAD) ? 0.0f : sinf(animTime * 4.0f + g->time) * 1.5f;
    float cx = g->pos.x;
    float cy = g->pos.y + bodyWave;
    float tailWave = g->tailSwing * 0.90f;
    float bellyFinSwing = (g->state == GUPPY_DEAD) ? 0.0f : g->finSwing * 0.75f;
    float mouthOpen = ComputeMouthOpen(g->eatFlash, animTime, g->time, 0.72f);

    Color body = ApplyStateTint((Color){248, 174, 60, 255}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);
    Color belly = ApplyStateTint((Color){255, 223, 128, 244}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);
    Color finPink = (g->state == GUPPY_DEAD)
        ? DeadFinColor((Color){244, 104, 174, 235})
        : ApplyStateTint((Color){244, 104, 174, 228}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);

    FishRig rig = BuildFishRig(cx, cy, dir, rot, scale,
        (FishRigLayout){40.0f, 22.0f, 47.0f, 30.0f, 24.0f, 10.0f, 22.0f, -4.0f, 4.0f, 9.0f, -30.0f, 9.0f},
        bodyWave, tailWave, bellyFinSwing, mouthOpen);

    DrawEllipse((int)cx, (int)(cy + 18.0f * scale), (int)(26.0f * scale), (int)(7.0f * scale), ColorAlpha(BLACK, 0.20f));
    DrawTailFan(&rig, dir, finPink, ColorAlpha(finPink, 0.94f), scale);

    DrawStaticDorsalFin(
        FacingPointScaled(cx, cy, -2.0f, -6.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 10.0f, -21.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 19.0f, -7.0f, dir, rot, scale),
        dir, finPink, scale);

    DrawGuppyBody(&rig, cx, cy, dir, rot, scale, body, belly);
    DrawSwingVentralFin(&rig, dir, finPink, scale);

    if (g->state == GUPPY_DEAD) {
        DrawDeadEye(cx, cy, dir, rot, scale, 18.0f, -4.0f);
    } else {
        DrawFishEye(rig.eye, dir, scale, WHITE, false);
        DrawCircleV((Vector2){rig.eye.x + 2.5f * dir * scale, rig.eye.y}, 4.2f * scale, BLACK);
        DrawCircleV((Vector2){rig.eye.x + 3.2f * dir * scale, rig.eye.y - 2.0f * scale}, 1.2f * scale, WHITE);
    }

    if (mouthOpen > 0.0f) {
        DrawFishMouth(&rig, dir, scale,
            ColorAlpha(body, 0.92f),
            ColorAlpha(belly, 0.92f),
            ColorAlpha((Color){120, 52, 70, 255}, 0.72f), 0);
    }

    if (g->eatFlash > 0.01f) {
        DrawRing(FacingPointScaled(cx, cy, 25.0f, 0.0f, dir, rot, scale),
            5.0f * scale, 10.0f * scale, 0, 360, 22,
            ColorAlpha(WHITE, g->eatFlash * 0.72f));
    }
}

static void DrawPiranhaDirect(const Carnivore *c, float animTime)
{
    if (!c || !c->active) return;

    float dir = (c->dir >= 0.0f) ? 1.0f : -1.0f;
    float rot = (c->state == CARNIVORE_DEAD) ? -1.0f : 1.0f;
    float scale = c->scale;
    float bodyWave = (c->state == CARNIVORE_DEAD) ? 0.0f : sinf(animTime * 3.8f + c->time * 0.65f) * 1.8f;
    float cx = c->pos.x;
    float cy = c->pos.y + bodyWave;
    float tailWave = c->tailSwing * 0.90f;
    float bellyFinSwing = (c->state == CARNIVORE_DEAD) ? 0.0f : c->finSwing * 0.72f;
    float mouthOpen = ComputeMouthOpen(c->eatFlash, animTime, c->time * 0.7f, 1.00f);

    Color upper = ApplyStateTint((Color){35, 38, 42, 255}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);
    Color belly = ApplyStateTint((Color){196, 46, 42, 244}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);
    Color fin = (c->state == CARNIVORE_DEAD)
        ? DeadFinColor((Color){86, 90, 96, 235})
        : ApplyStateTint((Color){86, 90, 96, 224}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);

    FishRig rig = BuildFishRig(cx, cy, dir, rot, scale,
        (FishRigLayout){46.0f, 29.0f, 46.0f, 29.0f, 31.0f, 13.0f, 16.0f, -4.0f, -10.0f, 16.0f, -38.0f, 10.0f},
        bodyWave, tailWave, bellyFinSwing, mouthOpen);

    DrawEllipse((int)cx, (int)(cy + 20.0f * scale), (int)(34.0f * scale), (int)(8.0f * scale), ColorAlpha(BLACK, 0.24f));
    DrawTailFan(&rig, dir, fin, fin, scale);

    DrawStaticDorsalFin(
        FacingPointScaled(cx, cy, -8.0f, -18.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 8.0f, -35.0f, dir, rot, scale),
        FacingPointScaled(cx, cy, 22.0f, -8.0f, dir, rot, scale),
        dir, fin, scale);

    DrawCarnivoreBody(&rig, cx, cy, dir, rot, scale, upper, belly, (Color){28, 30, 32, 255});
    DrawSwingVentralFin(&rig, dir, fin, scale);

    DrawFishMouth(&rig, dir, scale,
        ColorAlpha(upper, 0.96f),
        ColorAlpha(belly, 0.96f),
        ColorAlpha((Color){88, 22, 24, 255}, 0.78f), 4);

    if (c->state == CARNIVORE_DEAD) {
        DrawFishEye(rig.eye, dir, scale, LIGHTGRAY, true);
        DrawDeadEye(cx, cy, dir, rot, scale, 14.0f, -6.0f);
    } else {
        DrawFishEye(rig.eye, dir, scale, (Color){248, 222, 92, 255}, false);
    }

    if (c->eatFlash > 0.01f) {
        DrawRing(FacingPointScaled(cx, cy, 45.0f, 4.0f, dir, rot, scale),
            8.0f * scale, 14.0f * scale, 0, 360, 24,
            ColorAlpha((Color){255, 220, 180, 255}, c->eatFlash * 0.72f));
    }
}

static void DrawUltravoreDirect(const Ultravore *u, float animTime)
{
    if (!u || !u->active) return;

    float dir = (u->dir >= 0.0f) ? 1.0f : -1.0f;
    float rot = (u->state == ULTRA_DEAD) ? -1.0f : 1.0f;
    float scale = u->scale;
    float bodyWave = (u->state == ULTRA_DEAD) ? 0.0f : sinf(animTime * 3.0f + u->time * 0.55f) * 2.0f;
    float cx = u->pos.x;
    float cy = u->pos.y + bodyWave;
    float tailWave = u->tailSwing * 0.88f;
    float bellyFinSwing = (u->state == ULTRA_DEAD) ? 0.0f : u->finSwing * 0.66f;
    float mouthOpen = ComputeMouthOpen(u->eatFlash, animTime, u->time * 0.5f, 1.15f);

    Color body = ApplyStateTint((Color){152, 166, 176, 255}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);
    Color backShade = ApplyStateTint((Color){106, 120, 132, 230}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);
    Color redTail = (u->state == ULTRA_DEAD)
        ? DeadFinColor((Color){236, 58, 50, 238})
        : ApplyStateTint((Color){236, 58, 50, 232}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);

    FishRig rig = BuildFishRig(cx, cy, dir, rot, scale,
        (FishRigLayout){68.0f, 35.0f, 56.0f, 38.0f, 46.0f, 24.0f, 22.0f, -16.0f, -8.0f, 20.0f, -52.0f, 14.0f},
        bodyWave, tailWave, bellyFinSwing, mouthOpen);

    DrawEllipse((int)cx, (int)(cy + 24.0f * scale), (int)(44.0f * scale), (int)(10.0f * scale), ColorAlpha(BLACK, 0.26f));
    DrawTailFan(&rig, dir, redTail, ColorAlpha(redTail, 0.94f), scale);

    DrawUltravoreBody(&rig, cx, cy, dir, rot, scale, body, backShade);
    DrawSwingVentralFin(&rig, dir, redTail, scale);

    DrawFishMouth(&rig, dir, scale,
        ColorAlpha(backShade, 0.96f),
        ColorAlpha(body, 0.96f),
        ColorAlpha((Color){95, 36, 36, 255}, 0.78f), 5);

    if (u->state == ULTRA_DEAD) {
        DrawFishEye(rig.eye, dir, scale, LIGHTGRAY, true);
        DrawDeadEye(cx, cy, dir, rot, scale, 4.0f, -14.0f);
    } else {
        DrawCircleV(rig.eye, 9.0f * scale, (Color){250, 48, 48, 255});
        DrawCircleV(rig.eye, 5.4f * scale, ColorAlpha((Color){255, 70, 70, 255}, 0.45f));
        DrawCircleV((Vector2){rig.eye.x + 2.5f * dir * scale, rig.eye.y}, 4.4f * scale, BLACK);
        DrawCircleV((Vector2){rig.eye.x + 1.5f * dir * scale, rig.eye.y - 2.2f * scale}, 1.6f * scale, WHITE);
    }

    if (u->eatFlash > 0.01f) {
        DrawRing(FacingPointScaled(cx, cy, 68.0f, 6.0f, dir, rot, scale),
            11.0f * scale, 18.0f * scale, 0, 360, 28,
            ColorAlpha((Color){255, 200, 170, 255}, u->eatFlash * 0.72f));
    }
}

void WarmFishSpriteCaches(void)
{
    for (int hungryIndex = 0; hungryIndex < 2; hungryIndex++) {
        for (int dirIndex = 0; dirIndex < 2; dirIndex++) {
            for (int frame = 0; frame < FISH_CACHE_FRAMES; frame++) {
                EnsureGuppySprite(hungryIndex, dirIndex, frame);
                EnsurePiranhaSprite(hungryIndex, dirIndex, frame);
                EnsureUltravoreSprite(hungryIndex, dirIndex, frame);
            }
        }
    }
}

void DrawGuppyPreview(const Guppy *g, float animTime)
{
    DrawGuppyDirect(g, animTime);
}

void DrawPiranhaPreview(const Carnivore *c, float animTime)
{
    DrawPiranhaDirect(c, animTime);
}

void DrawUltravorePreview(const Ultravore *u, float animTime)
{
    DrawUltravoreDirect(u, animTime);
}

void DrawGuppy(const Guppy *g, float animTime)
{
    if (!g || !g->active) return;
    if (g->state == GUPPY_DEAD || g->eatFlash > 0.01f) {
        DrawGuppyDirect(g, animTime);
        return;
    }

    int hungryIndex = (g->state == GUPPY_HUNGRY) ? 1 : 0;
    int dirIndex = (g->dir >= 0.0f) ? 1 : 0;
    int frame = QuantizePhaseFrame(animTime * 4.0f + g->time);
    EnsureGuppySprite(hungryIndex, dirIndex, frame);
    DrawCachedFishSprite(s_guppySprites[hungryIndex][dirIndex][frame], g->pos, g->scale);
}

void DrawPiranha(const Carnivore *c, float animTime)
{
    if (!c || !c->active) return;
    if (c->state == CARNIVORE_DEAD || c->eatFlash > 0.01f) {
        DrawPiranhaDirect(c, animTime);
        return;
    }

    int hungryIndex = (c->state == CARNIVORE_HUNGRY) ? 1 : 0;
    int dirIndex = (c->dir >= 0.0f) ? 1 : 0;
    int frame = QuantizePhaseFrame(animTime * 3.8f + c->time * 0.65f);
    EnsurePiranhaSprite(hungryIndex, dirIndex, frame);
    DrawCachedFishSprite(s_piranhaSprites[hungryIndex][dirIndex][frame], c->pos, c->scale);
}

void DrawUltravore(const Ultravore *u, float animTime)
{
    if (!u || !u->active) return;
    if (u->state == ULTRA_DEAD || u->eatFlash > 0.01f) {
        DrawUltravoreDirect(u, animTime);
        return;
    }

    int hungryIndex = (u->state == ULTRA_HUNGRY) ? 1 : 0;
    int dirIndex = (u->dir >= 0.0f) ? 1 : 0;
    int frame = QuantizePhaseFrame(animTime * 3.0f + u->time * 0.55f);
    EnsureUltravoreSprite(hungryIndex, dirIndex, frame);
    DrawCachedFishSprite(s_ultravoreSprites[hungryIndex][dirIndex][frame], u->pos, u->scale);
}
