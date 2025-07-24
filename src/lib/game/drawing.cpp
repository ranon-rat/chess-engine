#include <iostream>

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
                DrawRectangle(i * SQUARE_SIZE, j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, DARKGRAY);
            else
                DrawRectangle(i * SQUARE_SIZE, j * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
        }
    }
}
void ChessGame::DrawInitialPos()
{

    if (!not_selected)
    {
        DrawCircle(from.x * SQUARE_SIZE + 25, from.y * SQUARE_SIZE + 25, 25, RED);
        for (size_t i = 0; i < possible_moves.size(); i++)
        {
            BoardCoordinates &v = possible_moves[i];
            DrawCircle(v.x * SQUARE_SIZE + 25, v.y * SQUARE_SIZE + 25, 25, RED);
        }
    }
}

void ChessGame::DrawBoardPieces()
{

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {
            uint64_t is_attacked_mask = bitwise_board.potenital_attacks;
            uint64_t position_mask = 1ull << (j * 8 + i);
            bool is_attacked = is_attacked_mask & position_mask;

            // Draw the pieces
            TypePiece piece = pieces[j * 8 + i];

            switch (piece.piece)
            {
            case Pieces::PAWN:
                // now i need to make moves

                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("P", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);

                break;
            case Pieces::KNIGHT:

                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("N", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::BISHOP:

                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("B", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::ROOK:

                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("R", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::QUEEN:

                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("Q", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::KING:

                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("K", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);
                break;
            case Pieces::UNKOWN:
                DrawRectangle(i * SQUARE_SIZE + 10, j * SQUARE_SIZE + 10, 30, 30, piece.isWhite ? WHITE : BLACK);
                DrawText("?", i * SQUARE_SIZE + 20, j * SQUARE_SIZE + 20, 20, piece.isWhite ? BLACK : WHITE);
            default:
                break;
            }
            if (is_attacked)
            {
                DrawRectangle(i * SQUARE_SIZE + 5, j * SQUARE_SIZE + 5, 40, 40, {.r = 0, .g = 0, .b = 255, .a = SQUARE_SIZE});
            }
        }
    }
}

void ChessGame::SelectPieces()
{
    if (ready_to_promote || game_state != GameStates::CONTINUE)
    {
        return;
    }
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
                        possible_moves = board.GetMoves(from, bitwise_board);
                        std::cout << possible_moves.size() << " possible moves" << "\n";

                        not_selected = false;
                    }
                    else
                    {
                        to.x = x % 8;
                        to.y = x / 8;
                        bitwise_board = board.MakeMove(from, to, bitwise_board,TypeGame::User);
                        
                        pieces = board.GetPieces(bitwise_board);
                        ready_to_promote = board.IsReadyToPromote(bitwise_board);
                        game_state = board.CheckBoardState(bitwise_board);
                        std::cout << board.GetFen(bitwise_board) << "\n";
                        std::cout << "to y:         " << static_cast<int>(to.y) << "\n";
                        std::cout << "to x:         " << static_cast<int>(to.x) << "\n";
                        std::cout<< "king in check: "<<(bitwise_board.king_check?"true":"false")<<"\n";
                        not_selected = true;
                    }
                    break;
                }
            }
        }
    }
}
void ChessGame::ShowBasicInformation()
{
    float x_offset = 8 * SQUARE_SIZE + 10;
    DrawText(bitwise_board.white_to_move ? "white move" : "black move", x_offset, 10, 10, BLACK);
}

void ChessGame::PromotionPart()
{
    if (!ready_to_promote)
    {
        return;
    }
    float x_offset = 8 * SQUARE_SIZE + 10;

    float y_offset = 4 * SQUARE_SIZE;

    DrawText("1->Q 2->R 3->B 4->K", x_offset, y_offset, 10, BLACK);
    Pieces new_piece = NONE;
    switch (GetKeyPressed())
    {
    case KEY_ONE:
        new_piece = Pieces::QUEEN;
        break;
    case KEY_TWO:
        new_piece = Pieces::ROOK;
        break;
    case KEY_THREE:
        new_piece = Pieces::BISHOP;
        break;
    case KEY_FOUR:
        new_piece = Pieces::KNIGHT;
        break;
    default:
        break;
    }
    if (new_piece == NONE)
    {
        return;
    }
    bitwise_board = board.Promotion(to, bitwise_board, new_piece);
    pieces = board.GetPieces(bitwise_board);
    game_state = board.CheckBoardState(bitwise_board);
    std::cout << board.GetFen(bitwise_board) << "\n";

    ready_to_promote = false;
}

void ChessGame::DrawGameState()
{
    if (game_state == GameStates::CONTINUE)
    {
        return;
    }

    int font_size = 20;
    int x_offset = 10;
    if (game_state == GameStates::CHECKMATE)
    {

        DrawText("CHECKMATE BITCH", SQUARE_SIZE * 8 + x_offset, screenHeight / 2, font_size, BLACK);
        DrawText(bitwise_board.white_to_move ? "BLACK WINS" : "WHITE WINS", SQUARE_SIZE * 8 + x_offset, screenHeight / 2 + font_size + 5, 10, BLACK);
    }
    else if (game_state == GameStates::DRAW)
    {
        DrawText("DRAW", SQUARE_SIZE * 8 + x_offset, screenHeight / 2, font_size, BLACK);
    }
}