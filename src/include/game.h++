#ifndef GAME_HPP
#define GAME_HPP
#include <string>
#include "board.h++"

class ChessGame
{
private:
    BitWiseBoard bitwise_board;
    Board board;
    std::array<Rectangle, 64> squares;
    bool not_selected = true;
    BoardCoordinates from;
    BoardCoordinates to;

    double lastClickTime = 0.0;
    double clickCooldown = 0.2;
    std::array<TypePiece, 64> pieces;
    std::vector<BoardCoordinates> possible_moves;
public:
    ChessGame(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    void Update();
    void DrawCheckBoardSquares();
    void DrawBoardPieces();
    void SelectPieces();
    void DrawInitialPos();
    void ShowBasicInformation();
};
#endif