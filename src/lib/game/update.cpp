#include "game.h++"
#include "raylib/raylib.h"

void ChessGame::Update()
{

    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    DrawCheckBoardSquares();
    // Draw the board
    DrawInitialPos();
    DrawBoardPieces();
    SelectPieces();
    ShowBasicInformation();

    PromotionPart();
DrawGameState();
    EndDrawing();
}