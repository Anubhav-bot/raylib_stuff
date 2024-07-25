#include "./raylib/src/raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define CAM_SPEED 500

typedef struct Ground{
    int x;
    int y;
    int width;
    int height;
    int color;
} Ground;

int main() {
    int WIDTH = 800;
    int HEIGHT = 500;
    FILE *fpt;
    fpt = fopen("world_data.txt", "a+");
    if (fpt == NULL) return 1;

    InitWindow(WIDTH, HEIGHT, "Level Editor");
    SetTargetFPS(60);

    Vector2 cam_pos = (Vector2) {0, 0};

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.offset = (Vector2){ WIDTH/2.0f, HEIGHT/2.0f };
    camera.target = cam_pos;

    float delta;
    while (!WindowShouldClose()) {
        delta = GetFrameTime();

        camera.offset = (Vector2){ WIDTH/2.0f, HEIGHT/2.0f };
        camera.target = cam_pos;

        int create = 0;
        if(IsGestureDetected(GESTURE_TAP)) {
            create = 1;
            Vector2 mouse_pos = GetMousePosition();
            printf("%d, %d\n", (int)mouse_pos.x, (int)mouse_pos.y);
        }

        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        if(IsKeyDown(KEY_D)) {
            cam_pos.x += CAM_SPEED * delta;
        }
        if(IsKeyDown(KEY_A)) {
            cam_pos.x -= CAM_SPEED * delta;
        }
        if(IsKeyDown(KEY_S)) {
            cam_pos.y += CAM_SPEED * delta;
        }
        if(IsKeyDown(KEY_W)) {
            cam_pos.y -= CAM_SPEED * delta;
        }

        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(GRAY);
                DrawLine(0, 1000, 0, -1000, RED);
                DrawLine(1000, 0, -1000, 0, BLUE);

                DrawCircle(cam_pos.x, cam_pos.y, 5, GREEN);


            EndMode2D();


            char st[10];
            sprintf(st, "camera_pos: (%.2f, %.2f)", cam_pos.x, cam_pos.y);
            DrawText(st, 0, 0, 24, RED);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
