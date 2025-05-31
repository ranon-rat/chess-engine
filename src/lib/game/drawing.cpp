#include "game.h++"
#include "raylib/raylib.h"
void ChessGame::DrawCheckBoardSquares()
{
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {

            // Draw the squares
            if ((i + j) % 2 == 0)
                DrawRectangle(i * 50, j * 50, 50, 50, DARKGRAY);
            else
                DrawRectangle(i * 50, j * 50, 50, 50, LIGHTGRAY);
        }
    }
}
    void ChessGame::DrawInitialPos(){
        if(!not_selected){
            DrawCircle(from.x*50+25,from.y*50+25,25,RED);
           possible_moves=board.GetMoves(from,bitwise_board);
           for(BoardCoordinates &v :possible_moves){
                        DrawCircle(v.x*50+25,v.y*50+25,25,RED);

           }
        }
    }

void ChessGame::DrawBoardPieces()
{

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {

            // Draw the pieces
            TypePiece piece = pieces[j * 8 + i];
            switch (piece.piece)
            {
            case Pieces::PAWN:
                // now i need to make moves

                DrawRectangle(i * 50 + 10, j * 50 + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("P", i * 50 + 20, j * 50 + 20, 20, piece.isWhite ? BLACK : WHITE);

                break;
            case Pieces::KNIGHT:

                DrawRectangle(i * 50 + 10, j * 50 + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("N", i * 50 + 20, j * 50 + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::BISHOP:

                DrawRectangle(i * 50 + 10, j * 50 + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("B", i * 50 + 20, j * 50 + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::ROOK:

                DrawRectangle(i * 50 + 10, j * 50 + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("R", i * 50 + 20, j * 50 + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::QUEEN:

                DrawRectangle(i * 50 + 10, j * 50 + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("Q", i * 50 + 20, j * 50 + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::KING:

                DrawRectangle(i * 50 + 10, j * 50 + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("K", i * 50 + 20, j * 50 + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            default:
                break;
            }
        }
    }
}

void ChessGame::SelectPieces()
{

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        double currentTime = GetTime();
        if (currentTime - lastClickTime > clickCooldown)
        {
            lastClickTime = currentTime;

            Vector2 mousePos = GetMousePosition();
            for (int x = 0; x < 64; x++)
            {
                if (CheckCollisionPointRec(mousePos, squares[x]))
                {
                    DrawRectangle(squares[x].x, squares[x].y, squares[x].width, squares[x].height, RED);

                    if (not_selected)
                    {
                        from.x = x % 8;
                        from.y = x / 8;
                        not_selected = false;
                    }
                    else
                    {
                        to.x = x % 8;
                        to.y = x / 8;
                        bitwise_board = board.MakeMove(from, to, bitwise_board);
                        pieces = board.GetPieces(bitwise_board);
                        not_selected = true;
                    }
                    break;
                }
            }
        }
    }
}
void ChessGame::ShowBasicInformation(){
    float x_offset=8*50+10;
    DrawText(bitwise_board.white_to_move?"white move":"black move",x_offset,10,10,BLACK);


}