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
    enum TypeFilter
    {
        Legal = 0,
        Defendable,
        Possible
    };

public:
    // these are for interal usage
    std::array<TypePiece, 64> GetPieces(BitWiseBoard &board); // this will return the pieces in the board
    BitWiseBoard BuildFromFEN(std::string fen);
    std::vector<BoardCoordinates> GetMoves(BoardCoordinates piece,const BitWiseBoard &board, TypeFilter filter = Legal); // esto deberia de retornarme un uint64_t con los movimientos legales de la pieza
    BitWiseBoard MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board);                   // i make reference to the board in that specific square
    // so this one, will be used to getting general information from the board :)
    TypePiece GetPieceFromCoord(BoardCoordinates from, const BitWiseBoard &board);
public:
    // obviously this is information is useful for knowing what its happening here :)
    bool EnemySquares(BoardCoordinates from, const BitWiseBoard &board);
    bool FriendSquares(BoardCoordinates from, const BitWiseBoard &board);
    bool OcuppiedSquares(BoardCoordinates from, const BitWiseBoard&board);
private:
    // this is for the rook, bishop, and queen
    void LineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter = Legal);
    // king, knight
    void OneLineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter = Legal);
    // only for the pawn
    void PawnMoves(BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter = TypeFilter::Legal);
    // Castling
    void CastlingMoves(BoardCoordinates origin,const BitWiseBoard&board,std::vector<BoardCoordinates> &moves);
    uint64_t GetUtilizedSquares(const BitWiseBoard&board);

    private:
    void MoveRook(BoardCoordinates from,BoardCoordinates to,BitWiseBoard&new_board,const BitWiseBoard&board,uint64_t initial_mask,uint64_t target_mask);
    void MovePawn(BoardCoordinates from,BoardCoordinates to,BitWiseBoard&new_board,const BitWiseBoard&board,uint64_t initial_mask,uint64_t target_mask,int direction);
        void EatPawnEnPassant(BoardCoordinates from,BoardCoordinates to,BitWiseBoard&new_board,const BitWiseBoard&board,uint64_t initial_mask,uint64_t target_mask,int direction);
    private:

    std::array<std::vector<Move>, Pieces::PIECE_COUNT> m_possible_moves; // this is the vector of possible moves
};

#endif