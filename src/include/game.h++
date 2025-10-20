#ifndef GAME_HPP
#define GAME_HPP

#include <array>
#include <string>
#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include "board-api.h++"
#include "engine.h++"
#include "raylib/raylib.h"
#define screenWidth 800
#define screenHeight 450
#define SQUARE_SIZE 50
class ChessGame
{
private:
    BoardAPI api;
    bool we_are_white=true;
    BitWiseBoard bitwise_board;
    std::array<Rectangle, 64> squares;
    bool not_selected = true;
    BoardCoordinates from;
    BoardCoordinates to;
    Engine engine;
    bool ready_to_promote = false;
    double lastClickTime = 0.0;
    double clickCooldown = 0.2;
    std::array<TypePiece, 64> pieces;
    MaxMovesArray possible_moves;
    GameStates game_state = GameStates::CONTINUE;

public:
    ChessGame(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    void Update();
    void DrawCheckBoardSquares();
    void DrawBoardPieces();
    void SelectPieces();
    void DrawInitialPos();
    void ShowBasicInformation();
    void PromotionPart();
    void DrawGameState();
};
#endif