#include "./raylib/src/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CAM_SPEED 500
#define WIDTH 800
#define HEIGHT 500

typedef struct Ground{
    int x;
    int y;
    int width;
    int height;
} Ground;

int main() {
    FILE *fpt;

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


        //Writing data
        // int create = 0;
        if(IsGestureDetected(GESTURE_TAP)) {
            // create = 1;
            Vector2 mouse_pos = GetMousePosition();
            mouse_pos.x = (int)cam_pos.x + (int)mouse_pos.x - WIDTH / 2; //TOP LEFT POSITION FOR GROUND ENTITY
            mouse_pos.y = (int)cam_pos.y + (int)mouse_pos.y - HEIGHT / 2; //BUT ONLY THE CENTER POSITION IS SAVED TO FILE

            Ground g = {(int)mouse_pos.x, (int)mouse_pos.y, 100, 30};
            printf("(%d, %d)\n", (int)mouse_pos.x, (int)mouse_pos.y);

            fpt = fopen("world_data.txt", "a");
            if (fpt == NULL) return 1;
            fprintf(fpt, "%d %d %d %d\n", g.x-g.width/2, g.y-g.height/2, g.width, g.height); //CENTER POSITION 
            fclose(fpt);

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


        //Reading data
        Rectangle r[10];

        fpt = fopen("world_data.txt", "a+");
        if (fpt == NULL) return 1;

        for(int i = 0; i < 10; i++) {
            fscanf(fpt, "%f %f %f %f", &r[i].x, &r[i].y, &r[i].width, &r[i].height); //TODO
        }

        fclose(fpt);
       

        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(GRAY);
                DrawLine(0, 1000, 0, -1000, RED);
                DrawLine(1000, 0, -1000, 0, BLUE);

                DrawCircle(cam_pos.x, cam_pos.y, 5, GREEN);

                for (int i = 0; i < 10; i++) {
                    DrawRectangleRec(r[i], SKYBLUE);
                }
        
            EndMode2D();


            char st[10];
            sprintf(st, "camera_pos: (%.2f, %.2f)", cam_pos.x, cam_pos.y);
            DrawText(st, 0, 0, 24, RED);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
