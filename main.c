#include "raylib.h"
#include "raylib/src/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 500

#define GRAVITY 500

#define PLAYER_SPEED 100
#define PLAYER_JMP_SPEED 350

struct Player {
    float x;
    float y;
    float width;
    float height;
    float vSpeed;
    bool canJump;
};

typedef struct Ground {
    int x;
    int y;
    int width;
    int height;
} Ground;

int main() {
    FILE *fpt;
    Ground g[10];
    fpt = fopen("world_data.txt", "r");
    for(int i = 0; i < 10; i++)
    {
        fscanf(fpt, "%d %d %d %d", &g[i].x, &g[i].y, &g[i].width, &g[i].height);
    }
    fclose(fpt);

    InitWindow(WIDTH, HEIGHT, "THIS TIME FOR SURE");

    struct Player p = {0, -90, 30, 30, 0, 0};
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = (Vector2){p.x, p.y};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};

    float delta;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        delta = GetFrameTime();

        camera.target = (Vector2){p.x, p.y};
        camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};

        camera.zoom += ((float)GetMouseWheelMove()*0.05f);
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            p.x += PLAYER_SPEED * delta;
        }

        if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            p.x -= PLAYER_SPEED * delta;
        }

        if(IsKeyDown(KEY_SPACE) && p.canJump) {
            p.vSpeed = -PLAYER_JMP_SPEED;
            p.canJump = false;
        }
        
        bool isColliding = false;
        for(int i = 0; i < 10; i++) {
            if(CheckCollisionRecs((Rectangle){p.x, p.y, p.width, p.height}, (Rectangle){g[i].x, g[i].y, g[i].width, g[i].height}))
            {
                isColliding = true;
                p.vSpeed = 0;
                p.y = g[i].y - p.height;
            }
        }

        if(!isColliding) {
            p.vSpeed += GRAVITY * delta;
            p.y += p.vSpeed * delta;
            p.canJump = false;
        }
        else {
            p.canJump = true;
        }

        BeginDrawing();

            BeginMode2D(camera);
                ClearBackground(DARKGRAY);

                for(int i = 0; i < 10; i++) {
                    DrawRectangle(g[i].x, g[i].y, g[i].width, g[i].height, SKYBLUE);
                }

                
                DrawLine(0, 1000, 0, -1000, RED);
                DrawLine(1000, 0, -1000, 0, BLUE);

                DrawRectangle(p.x, p.y, p.width, p.height, GREEN);
            EndMode2D();

            DrawFPS(WIDTH - 100, 10);


        EndDrawing();

    }
    CloseWindow();
    return 0;
}
