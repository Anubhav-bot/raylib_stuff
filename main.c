#include <stdbool.h>
#include "raylib/src/raylib.h"
#include <stdio.h>

#define G 9.8

typedef struct Player {
    Vector2 position;
    float speed;
    float vSpeed;
    bool canJump;
}Player;

int main(void) {
    const int WIDTH = 800;
    const int HEIGHT = 500;


    InitWindow(800, 500, "IDK");

    Player p = { 0 };
    p.position = (Vector2){100, 100};
    p.speed = 50.0f;
    p.vSpeed = 100.0f;

    Camera2D camera = { 0 };
    camera.zoom = 1;
    camera.rotation = 0;

    SetTargetFPS(60);

    camera.target = p.position;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        //UPDATE
        camera.offset = (Vector2){WIDTH/2, HEIGHT/2};

        if (IsKeyDown(KEY_A)) p.position.x -= p.speed * delta;
        else if (IsKeyDown(KEY_D)) p.position.x += p.speed * delta;
        if (IsKeyPressed(KEY_SPACE) && p.canJump) {
            p.position.y -= p.speed;
        }

        p.vSpeed += G*delta;
        p.position.y += p.vSpeed*delta;

        // if (p.position.y >= 400) p.position.y = HEIGHT - 100;
        if (p.position.y >= 200) 

        //DRAW
        BeginDrawing();
        ClearBackground(GRAY);
        BeginMode2D(camera);

        DrawRectangle(p.position.x, p.position.y, 20, 20, GREEN);

        EndMode2D();
        EndDrawing();
        

    
    }
    return 0;
}
