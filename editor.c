#include "./raylib/src/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "./common.h"

#define CAM_SPEED 500
#define WIDTH_INITIAL 800
#define HEIGHT_INITIAL 500

typedef struct Ground{
    int x;
    int y;
    int width;
    int height;
} Ground;

typedef struct InputBox {
    int x;
    int y;
    int width;
    int height;
    char input[11];
    int max_count;
    int cur_count;
    char label[10];
    bool focus;
} InputBox;

void debug(int obstacle_counter, Vector2 cam_pos);
void UpdateBoxes(InputBox boxes[], size_t box_count);
void DeleteData(FILE *fpt, int l);

int main() {
    FILE *fpt;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 
    InitWindow(WIDTH_INITIAL, HEIGHT_INITIAL, "Level Editor");
    int WIDTH = GetScreenWidth();
    int HEIGHT = GetScreenHeight();
    SetTargetFPS(60);

    size_t box_count = 3;
    int box_index = 0;
    InputBox boxes[box_count];
    for(size_t i = 0; i < box_count; i++) {
        boxes[i].x = 290 + 110 * box_index++ + 10;
        boxes[i].y = 20;
        boxes[i].width = 100;
        boxes[i].height = 30;
        boxes[i].max_count = 10;
        boxes[i].cur_count = 3;
        boxes[i].focus = false;

        strcpy(boxes[i].input, "100");
        strcpy(boxes[i].input, "100");
        strcpy(boxes[i].label, "Unset: ");
    }

    strcpy(boxes[0].label, "Width: ");
    strcpy(boxes[1].label, "Height: ");
    strcpy(boxes[2].label, "Type: ");
    strcpy(boxes[1].input, "10");
    strcpy(boxes[2].input, "0");

    Vector2 cam_pos = (Vector2) {0, 0};

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.offset = (Vector2){ WIDTH/2.0f, HEIGHT/2.0f };
    camera.target = cam_pos;

    float delta;
    bool enableDebug = true;

    while (!WindowShouldClose()) {
        delta = GetFrameTime();

        camera.offset = (Vector2){ GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
        camera.target = cam_pos;

        UpdateBoxes(boxes, box_count);
        int obs_w = TextToInteger(boxes[0].input);
        int obs_h = TextToInteger(boxes[1].input);
        int obs_type = TextToInteger(boxes[2].input);

        //Writing data
        // int create = 0;
        if(IsGestureDetected(GESTURE_TAP) && GetMouseY() > 70) {
            // create = 1;
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);

            Ground g = {(int)mouse_pos.x, (int)mouse_pos.y, obs_w, obs_h};
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
        if(IsKeyPressed(KEY_GRAVE)) {
            enableDebug = !enableDebug;
        }
        if(IsKeyDown(KEY_C)) {
            DeleteData(fpt, 0);
        }

        if(IsKeyDown(KEY_LEFT_CONTROL)) {
            // if(IsKeyPressed(KEY_Z)) DeleteData(fpt, obstacle_counter);
            if(IsKeyPressed(KEY_Z)) DeleteData(fpt, -1);
        }

        //Reading data
        fpt = fopen("world_data.txt", "a+");
        if (fpt == NULL) return 1;

        int obstacle_counter = 0;
        while(fscanf(fpt, "%*d %*d %*d %*d") != EOF){
            obstacle_counter++;
        }
        rewind(fpt);


        Rectangle r[obstacle_counter];

        for(int i = 0; i < obstacle_counter; i++) {
            fscanf(fpt, "%f %f %f %f", &r[i].x, &r[i].y, &r[i].width, &r[i].height);
        }

        

        fclose(fpt);
       

        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(GRAY);
                DrawLine(0, 1000, 0, -1000, RED);
                DrawLine(1000, 0, -1000, 0, BLUE);
        

                DrawCircle(cam_pos.x, cam_pos.y, 5, GREEN);

                for (int i = 0; i < obstacle_counter; i++) {
                    DrawRectangleRec(r[i], SKYBLUE);
                }
        
            EndMode2D();

            DrawRectangle(0, 0, GetScreenWidth(), 70, GetColor(0x181818));
            if(enableDebug) debug(obstacle_counter, cam_pos);

            for(size_t i = 0; i < box_count; i++) {
                InputBox b = boxes[i];
                DrawRectangle(b.x, b.y, b.width, b.height, GetColor(0xEEEEEEFF));
                DrawRectangleLines(b.x, b.y, b.width, b.height, (b.focus)? RED: BLACK);
                DrawText(b.label, b.x, b.y - 24, 24, LIGHTGRAY);
                DrawText(b.input, b.x + 5, b.y + 5, 24, GRAY);
            }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void UpdateBoxes(InputBox boxes[], size_t box_count) {
    int cursor = MOUSE_CURSOR_DEFAULT;
    for(size_t i = 0; i < box_count; i++) {
        InputBox *b = boxes + i;
        bool hovering = CheckCollisionPointRec(GetMousePosition(), (Rectangle){b->x, b->y, b->width, b->height});
        if(hovering) {
            cursor = MOUSE_CURSOR_IBEAM;
            if(IsGestureDetected(GESTURE_TAP)){
                b->focus = true;
            }
        }

        else if(IsGestureDetected(GESTURE_TAP)) {
            b->focus = false;
        }

        if(b->focus) {
            int key = GetCharPressed();
            while(key > 0) {
                if ((key >= 48) && (key <= 57) && (b->cur_count < b-> max_count)) {
                    b->input[b->cur_count + 1] = '\0';
                    b->input[b->cur_count++] = (char)key;
                }

                key = GetCharPressed();
            }

            if(IsKeyPressed(KEY_BACKSPACE)) {
                b->cur_count--;
                if (b->cur_count < 0) b->cur_count = 0;
                b->input[b->cur_count] = '\0';
            }

        }
    }
    SetMouseCursor(cursor);
}

void DeleteData(FILE *fpt, int l) {
    switch(l) {
        case 0:
            fpt = fopen("world_data.txt", "w");
            fclose(fpt);
            break;
            
        default:
            FILE *tmp = fopen("tmp.txt", "w");
            fpt = fopen("world_data.txt", "r");
            char str[100];

            int no_of_lines = 0;
            while(fgets(str, 100, fpt) != NULL){
                no_of_lines++;
            }
            rewind(fpt);

            if(l > no_of_lines) {
                printf("Invalid argument 'l: %d' to DeleteData()\n", l);
                exit(1);
            }

            printf("DEBUG: No. of Lines: %d\n", no_of_lines);
            if(l == -1) l = no_of_lines;
            printf("DEBUG: Line to remove: %d\n", l);

            int c = 1;
            while(fgets(str, 100, fpt) != NULL) {
                if(c != l) {
                    printf("\nDEBUG: Writing line: %d\n", c);
                    puts(str);
                    fputs(str, tmp);
                }
                else {
                    printf("\nDEBUG: Skipping line: %d\n", c);
                    puts(str);
                }
                c++;
            }
            fclose(tmp);
            fclose(fpt);

            remove("world_data.txt");
            rename("tmp.txt", "world_data.txt");
            break;
    }
}

void debug(int obstacle_counter, Vector2 cam_pos) {/*{{{*/
    char text[100];
    int index = 0;
    int size = 24;
    int x_pos = 10;

    DrawFPS(GetScreenWidth() - 100, 10);

    sprintf(text, "Obs: %d\n", obstacle_counter);
    DrawText(text, x_pos, index++ * size, size, RED);
    // DrawText(TextFormat("Obs: %d\n", obstacle_counter), x_pos, index++ * size, size, RED);

    sprintf(text, "camera_pos: (%.2f, %.2f)", cam_pos.x, cam_pos.y);
    DrawText(text, x_pos, index++ * size, size, RED);
}/*}}}*/
