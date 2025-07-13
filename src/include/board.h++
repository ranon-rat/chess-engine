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
    BitWiseBoard Promotion(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard&board,TypePiece new_piece); // so you select the new piece :)
public:
    // obviously this is information is useful for knowing what its happening here :)
    bool EnemySquares(BoardCoordinates from, const BitWiseBoard &board, bool is_white);
    bool FriendSquares(BoardCoordinates from, const BitWiseBoard &board, bool is_white);
    bool OcuppiedSquares(BoardCoordinates from, const BitWiseBoard &board);

private:
    // this is for the rook, bishop, and queen
    void LineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter = Legal);
    // king, knight
    void OneLineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &movesbool, bool is_white, TypeFilter filter = Legal);
    // only for the pawn
    void PawnMoves(BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter = TypeFilter::Legal);
    // Castling
    void CastlingMoves(BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, uint64_t attack_mask, TypeFilter filter);
    uint64_t GetUtilizedSquares(const BitWiseBoard &board);

private: //  simulation shit
    uint64_t GetAttackedSquares(const BitWiseBoard &board, std::optional<bool> is_white = std::nullopt);

private:
    // this will return a mask that will tell us if by doing that we will attack our king or not, basically thats all we are going to do :)
    bool IsChecked(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board, bool from_white);

private:
    void MoveRook(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask);
    void MovePawn(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int8_t direction);
    void EatPawnEnPassant(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int8_t direction);
    void MoveKing(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask);

private:
    GameStates CheckBoardState(const BitWiseBoard &board);

private:
    std::array<std::vector<Move>, Pieces::PIECE_COUNT> m_possible_moves; // this is the vector of possible moves
};

#endif