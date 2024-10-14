#include "raylib/src/raylib.h"
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define BLOCK_SIZE 5

#define UPDATE_TIME 400
#define MAX_VELOCITY 40

class Sand {
    public:
    int v;
    int val;
    int counter;
    Color c;
    Sand() : v(2), val(0), counter(0), c(BLACK) {};
    Sand(int v, int val) : v(v), val(val), counter(0) {};
};

void swapSands(Sand (*matrix)[SCREEN_HEIGHT/BLOCK_SIZE], int loc1x, int loc1y, int loc2x, int loc2y) {
    matrix[loc1x][loc1y].val = 0; // value swaps
    matrix[loc2x][loc2y].val = 1;
    int tempv = matrix[loc1x][loc1y].v; // velocity swaps
    matrix[loc1x][loc1y].v = matrix[loc2x][loc2y].v;
    matrix[loc2x][loc2y].v = tempv;
    Color tempc = matrix[loc1x][loc1y].c; // color swaps
    matrix[loc1x][loc1y].c = matrix[loc2x][loc2y].c;
    matrix[loc2x][loc2y].c = tempc;
}

void updateMatrixLoc(Sand (*matrix)[SCREEN_HEIGHT/BLOCK_SIZE], int i, int j) {
    if (j == SCREEN_HEIGHT/BLOCK_SIZE - 1) {
        return;
    }
    if (matrix[i][j].val == 1) {
        if (matrix[i][j+1].val == 0) { // fill bottom by default
            swapSands(matrix, i, j, i, j+1);
        } else if (i != SCREEN_WIDTH/BLOCK_SIZE - 1 && matrix[i+1][j+1].val == 0) { // move to left if both bottom and right are taken
            swapSands(matrix, i, j, i+1, j+1);
        } else if (i != 0 && matrix[i-1][j+1].val == 0) { // if all fails, sand stays still
            swapSands(matrix, i, j, i-1, j+1);
        }
    }
}

void updateColors(Color* c) {
    if (IsKeyDown(KEY_ONE)) {
        *c = BLACK;
    }
    if (IsKeyDown(KEY_TWO)) {
        *c = RED;
    }
    if (IsKeyDown(KEY_THREE)) {
        *c = GREEN;
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Falling Sand");
    Sand matrix[SCREEN_WIDTH/BLOCK_SIZE][SCREEN_HEIGHT/BLOCK_SIZE] = {Sand()};
    Color col = BLACK;

    while(!WindowShouldClose()) {
        updateColors(&col);
        if (IsMouseButtonDown(0)) {
            Vector2 mousepos = GetMousePosition();
            int x = mousepos.x / BLOCK_SIZE;
            int y = mousepos.y / BLOCK_SIZE;
            matrix[x][y].val = 1;
            matrix[x][y].c = col;
        }
        
        BeginDrawing();
        /* <-------------------!-------------------> */
        ClearBackground(RAYWHITE);
        DrawText("Hello World", 10, 10, 20, BLACK);
        for (int i = 0; i < SCREEN_WIDTH/BLOCK_SIZE ; i++) {
            for (int j = 0; j < SCREEN_HEIGHT/BLOCK_SIZE ; j++) {
                if (matrix[i][j].val == 1) { 
                    DrawRectangle(i*BLOCK_SIZE, j*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, matrix[i][j].c);
                    matrix[i][j].counter++;
                }
                if (matrix[i][j].counter >= UPDATE_TIME / matrix[i][j].v) {
                    if (matrix[i][j].v <= MAX_VELOCITY) matrix[i][j].v++;
                    updateMatrixLoc(matrix, i, j);
                    matrix[i][j].counter = 0;
                }
            }
        }
        /* <-------------------!-------------------> */
        EndDrawing();
    }

    CloseWindow();
    return 0;
}