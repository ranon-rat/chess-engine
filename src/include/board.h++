#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <cstdint>
#include <string>
#include <array>
#include "BitWiseBoard.h++"
#include "raylib/raylib.h"
#include "PiecesAndMoves.h++"

struct TypePiece
{
    Pieces piece;
    bool isWhite; // true for white, false for black
};
struct BoardCoordinates
{
    int x;
    int y;
};
class Board // this is the playing board :)
{
public:
    Board();

public:
    // these are for interal usage
    std::array<TypePiece, 64> GetPieces(BitWiseBoard &board); // this will return the pieces in the board
    BitWiseBoard BuildFromFEN(std::string fen);
    std::vector<BoardCoordinates> legalMoves(BoardCoordinates piece, BitWiseBoard &board);        // esto deberia de retornarme un uint64_t con los movimientos legales de la pieza
    BitWiseBoard MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board); // i make reference to the board in that specific square
    // so this one, will be used to getting general information from the board :)
    TypePiece GetPieceFromCoord(BoardCoordinates from, const BitWiseBoard &board);

    bool AttackableSquares(BoardCoordinates from,const BitWiseBoard&board);
    bool FriendSquares(BoardCoordinates from, const BitWiseBoard&board);
    
    enum TypeFilter{
        Legal=0,
        Defendable,
        Possible
    };
private:

    
    void LineMoves(Pieces piece, BoardCoordinates origin, BitWiseBoard &board, std::vector<BoardCoordinates> &moves,TypeFilter filter=Legal);
    void OneLineMoves(Pieces piece,BoardCoordinates origin, BitWiseBoard &board,std::vector<BoardCoordinates> &moves,TypeFilter filter=Legal);
    void PawnMove(BoardCoordinates origin,const BitWiseBoard&board,std::vector<BoardCoordinates> &moves,TypeFilter filter=TypeFilter::Legal);
    void AlterLongMove(BoardCoordinates origin, BitWiseBoard&board);
     std::array<std::vector<Move>, Pieces::PIECE_COUNT> m_possible_moves; // this is the vector of possible moves
};

#endif