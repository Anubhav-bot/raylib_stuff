#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH_INITIAL 800
#define HEIGHT_INITIAL 500

#define GRAVITY 400

#define PLAYER_SPEED 200.0f
#define PLAYER_JMP_SPEED 350.0f

typedef struct Player {
    float x;
    float y;
    float width;
    float height;
    float vSpeed;
    bool canJump;
}Player;

typedef struct Ground {
    int x;
    int y;
    int width;
    int height;
} Ground;

void debug(int obstacle_counter, bool isColliding, Player *p) {
    char text[100];
    int index = 0;
    int size = 24;
    int x_pos = 10;

    DrawFPS(GetScreenWidth() - 100, 10);

    sprintf(text, "Obs: %d\n", obstacle_counter);
    DrawText(text, x_pos, index++ * size, size, RED);

    if(isColliding) DrawText("Collision: True", x_pos, index++ * size, size, RED);
    else DrawText("Collision: False", x_pos, index++ * size, size, RED);

    sprintf(text, "Pos: (%d, %d)\n", (int)p->x, (int)p->y);
    DrawText(text, x_pos, index++ * size, size, RED);

    sprintf(text, "vSpeed: %.2f\n", p->vSpeed);
    DrawText(text, x_pos, index++ * size, size, RED);
}

int main() {
    FILE *fpt;

//UNCOMMENT TO MAKE STATIC
    // fpt = fopen("world_data.txt", "r");
    // int obstacle_counter = 0;
    // while(fscanf(fpt, "%*d %*d %*d %*d") != EOF){
    //     obstacle_counter++;
    // }
    //
    // Ground g[obstacle_counter];
    // rewind(fpt);
    //
    // 
    // for(int i = 0; i < obstacle_counter; i++) {
    //     fscanf(fpt, "%d %d %d %d", &g[i].x, &g[i].y, &g[i].width, &g[i].height);
    // }
    // fclose(fpt);
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(WIDTH_INITIAL, HEIGHT_INITIAL, "THIS TIME FOR SURE");
    int WIDTH = GetScreenWidth();
    int HEIGHT = GetScreenHeight();

    Player p = {0, -90, 30, 30, 0, 0};
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = (Vector2){p.x, p.y};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};

    Texture2D bg = LoadTexture("bg.png");

    float delta;
    SetTargetFPS(60);

    bool enableDebug = true;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();


        //DEV (DYNAMIC LOADING NOT REQUIRED FOR FINAL VERSION) okay to remove
        // make sure to uncomment the static block above
            fpt = fopen("world_data.txt", "r");
            int obstacle_counter = 0;
            while(fscanf(fpt, "%*d %*d %*d %*d") != EOF){
                obstacle_counter++;
            }

            Ground g[obstacle_counter];
            rewind(fpt);

            
            for(int i = 0; i < obstacle_counter; i++) {
                fscanf(fpt, "%d %d %d %d", &g[i].x, &g[i].y, &g[i].width, &g[i].height);
            }
            fclose(fpt);
         //

        if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            p.x += PLAYER_SPEED * delta;
        }

        if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            p.x -= PLAYER_SPEED * delta;
        }

        if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            p.vSpeed += 10;
        }

        if(IsKeyDown(KEY_SPACE) && p.canJump) {
            p.vSpeed = -PLAYER_JMP_SPEED;
            p.canJump = false;
        }

        if(IsKeyPressed(KEY_R)) {
            p.x = 0;
            p.y = -90;
            p.vSpeed = 0;
            camera.zoom = 1.0f;
        }

        if(IsKeyPressed(KEY_GRAVE)) {
            enableDebug = !enableDebug;
        }

        
        bool isColliding = false;
        for(int i = 0; i < obstacle_counter; i++) {
            Ground *gi = g + i;
            if(
                gi->x <= p.x + p.width &&
                gi->x + gi->width >= p.x &&
                gi->y >= p.y + p.height&&
                gi->y <= p.y + p.height + p.vSpeed*delta
            )
            {
                isColliding = true;
                p.vSpeed = 0.0f;
                p.y = g[i].y - p.height;
            }
        }

        if(!isColliding) {
            p.y += p.vSpeed * delta;
            p.vSpeed += GRAVITY * delta;
            p.canJump = false;
        } else {
            p.canJump = true;
        }

        camera.target = (Vector2){p.x, p.y};
        camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};

        camera.zoom += ((float)GetMouseWheelMove()*0.05f);
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        BeginDrawing();
            DrawTexture(bg, 0, 0, GRAY);
        
            BeginMode2D(camera);

                for(int i = 0; i < obstacle_counter; i++) {
                    DrawRectangle(g[i].x, g[i].y, g[i].width, g[i].height, SKYBLUE);
                }
                
                DrawLine(0, 1000, 0, -1000, RED);
                DrawLine(1000, 0, -1000, 0, BLUE);

                Rectangle playerRect = {p.x, p.y, p.width, p.height};
                DrawRectangleRec(playerRect, GREEN);
            EndMode2D();

            if(enableDebug) debug(obstacle_counter, isColliding, &p);
            

        EndDrawing();

    }

    UnloadTexture(bg);
    CloseWindow();
    return 0;
}
