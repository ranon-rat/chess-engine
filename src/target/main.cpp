#include <iostream>
#include "raylib/raylib.h"
#include "game.h++"
int main()
{

    std::cout << "Hello, World!" << std::endl;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    // Initialize the board
    ChessGame game;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        game.Update();
    }
    CloseWindow();

    return 0;
}