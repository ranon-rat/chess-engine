#include <iostream>
#include "raylib/raylib.h"
#include "game.h++"
int main()
{

    std::cout << "Hello, World!" << std::endl;

    InitWindow(screenWidth, screenHeight, "Chess game killer epic God of time");
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