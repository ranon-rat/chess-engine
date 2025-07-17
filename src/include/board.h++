#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <cstdint>
#include <string>
#include <array>
#include <optional>
#include <algorithm>
#include "BitWiseBoard.h++"
#include "raylib/raylib.h"
#include "PiecesAndMoves.h++"
#include "others.h++"

typedef std::array<std::vector<Move>, Pieces::PIECE_COUNT-1>  ArrayPieces;
ArrayPieces InitPossibleMoves();
class Board // this is the playing board :)
{
public:
    Board();
    enum TypeFilter
    {
        Legal = 0,
        Defendable
    };

public:
    // these are for interal usage
    std::array<TypePiece, 64> GetPieces(BitWiseBoard &board); // this will return the pieces in the board
    BitWiseBoard BuildFromFEN(std::string fen);
    MaxMovesArray GetMoves(BoardCoordinates piece, const BitWiseBoard &board, std::optional<bool> is_white = std::nullopt, TypeFilter filter = Legal); // esto deberia de retornarme un uint64_t con los movimientos legales de la pieza
    BitWiseBoard MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board, bool simulation = false);                             // i make reference to the board in that specific square
    // so this one, will be used to getting general information from the board :)
    TypePiece GetPieceFromCoord(BoardCoordinates from, const BitWiseBoard &board);
    bool IsReadyToPromote(const BitWiseBoard &board);
    BitWiseBoard Promotion(BoardCoordinates from, const BitWiseBoard &board, Pieces new_piece, std::optional<bool> is_white = std::nullopt); // so you select the new piece :)
    GameStates CheckBoardState(const BitWiseBoard &board);

public:
    // obviously this is information is useful for knowing what its happening here :)
    bool EnemySquares(BoardCoordinates from, const BitWiseBoard &board, bool is_white);
    bool FriendSquares(BoardCoordinates from, const BitWiseBoard &board, bool is_white);
    bool OcuppiedSquares(BoardCoordinates from, const BitWiseBoard &board);
    uint64_t GetUtilizedSquares(const BitWiseBoard &board);
    bool IsChecked(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board, bool from_white);
    uint64_t GetZobrist(const BitWiseBoard &board);

private:
    // this is for the rook, bishop, and queen
    void lineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter = Legal);
    // king, knight
    void oneLineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &movesbool, bool is_white, TypeFilter filter = Legal);
    // only for the pawn
    void pawnMoves(BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter = TypeFilter::Legal);
    // Castling
    void castlingMoves(BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, uint64_t attack_mask, TypeFilter filter);

private: //  simulation shit
    uint64_t getAttackedSquares(const BitWiseBoard &board, std::optional<bool> is_white = std::nullopt);
    bool movementIsLegal(const BoardCoordinates &from, const BoardCoordinates &to, const BitWiseBoard &board);

private:
    void moveRook(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask);
    void movePawn(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int8_t direction);
    void moveKing(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask);
    void eatPawnEnPassant(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int8_t direction);
    void eatRook(BoardCoordinates to, uint64_t target_mask, BitWiseBoard &new_board, const BitWiseBoard &board);

private:
     const ArrayPieces m_possible_moves=InitPossibleMoves(); // this is the vector of possible moves
};

#endif