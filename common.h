#ifndef COMMON_H
#define COMMON_H

#include "./raylib/src/raylib.h"

typedef enum {
    GROUND = 0,
    PLATFORM_H = 1,
    PLATFORM_V = 2

} EnvType;

typedef struct EnvItem {
    float x;
    float y;
    float width;
    float height;
    Color color;
    EnvType type;
} EnvItem;

#endif // !COMMON_H
