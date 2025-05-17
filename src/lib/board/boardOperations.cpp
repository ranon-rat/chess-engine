#include "board.h++"
#include <iostream>
#include <bitset>
BitWiseBoard Board::MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board)
{
    BitWiseBoard new_board = board;
    // First i need to check if the piece is a legal move,
    // thats all :) XD

    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t target_mask = 1ULL << ((to.y * 8) + to.x);
    // now we have to check the kinds
    uint64_t occupied_squares = board.white_to_move ? board.white_pieces : board.black_pieces;

    if (!(piece_mask & board.utilized_squares)||!(piece_mask&occupied_squares))
    { // if its not in one of our pieces then just return 0 :)
        return new_board;
    }

    if (piece_mask & board.pawns)
    {
        new_board.pawns &= ~piece_mask;
        new_board.pawns |= target_mask;
    }
    else if (piece_mask & board.knights)
    {
        new_board.knights &= ~piece_mask;
        new_board.knights |= target_mask;
    }
    else if (piece_mask & board.bishops)
    {
        new_board.bishops &= ~piece_mask;
        new_board.bishops |= target_mask;
    }
    else if (piece_mask & board.rooks)
    {
        new_board.rooks &= ~piece_mask;
        new_board.rooks |= target_mask;
    }
    else if (piece_mask & board.queens)
    {
        new_board.queens &= ~piece_mask;
        new_board.queens |= target_mask;
    }
    else if (piece_mask & board.kings)
    {
        new_board.kings &= ~piece_mask;
        new_board.kings |= target_mask;
    }
    else if (piece_mask & board.kings)
    {
        new_board.kings &= ~piece_mask;
        new_board.kings |= target_mask;
    }
    if (target_mask & board.pawns)
    {
        new_board.pawns &= ~target_mask;
    }
    else if (target_mask & board.knights)
    {
        new_board.knights &= ~target_mask;
        new_board.knights |= target_mask;
    }
    else if (target_mask & board.bishops)
    {
        new_board.bishops &= ~target_mask;
        new_board.bishops |= target_mask;
    }
    else if (target_mask & board.rooks)
    {
        new_board.rooks &= ~target_mask;
        new_board.rooks |= target_mask;
    }
    else if (target_mask & board.queens)
    {
        new_board.queens &= ~target_mask;
        new_board.queens |= target_mask;
    }
    else if (target_mask & board.kings)
    {
        new_board.kings &= ~target_mask;
        new_board.kings |= target_mask;
    }
    if (board.white_to_move)
    {
        new_board.white_pieces &= ~piece_mask;
        new_board.white_pieces |= target_mask;
        new_board.black_pieces &= ~target_mask;
    }
    else
    {
        new_board.black_pieces &= ~piece_mask;
        new_board.black_pieces |= target_mask;
        new_board.white_pieces &= ~target_mask;
    }
    new_board.utilized_squares &= ~piece_mask;
    new_board.utilized_squares |= target_mask;

    new_board.white_to_move = !board.white_to_move;

    return new_board;
}

std::vector<BoardCoordinates> Board::legalMoves(BoardCoordinates piece, BitWiseBoard &board)
{

    // checking to//
    uint64_t occupied_squares = board.white_to_move ? board.white_pieces : board.black_pieces;
    // now we need to check if the piece is a pawn or not
    uint64_t piece_mask = 1ULL << ((piece.y * 8) + piece.x);
    std::vector<BoardCoordinates> moves;
    // hmmm
    // first lets check if the piece is one of us

    if (!(piece_mask & occupied_squares))
    {                 // if its not in one of our pieces then just return 0 :)
        return moves; // an empty vector
    }
    // avaible pieces
    // uint64_t avaible_pieces=0;
    if (piece_mask & board.pawns)
    {
    }
    else if (piece_mask & board.knights)
    {
    }
    else if (piece_mask & board.bishops)
    {
        LineMoves(BISHOP, piece, board,moves);
    }
    else if (piece_mask & board.rooks)
    {
        LineMoves(ROOK, piece, board,moves);
    }
    else if (piece_mask & board.queens)
    {
        LineMoves(QUEEN, piece, board,moves);
    }
    else if (piece_mask & board.kings)
    {
    }
    return moves;
}
uint64_t generateBitBoard(uint64_t &bitBoard, uint8_t x, uint8_t y)
{
    bitBoard |= 1ULL << ((y * 8) + x);
    return bitBoard;
}

uint64_t ThereIsSomething(int x, int y)
{
    return 0;
}
void Board::LineMoves(Pieces piece, BoardCoordinates origin, BitWiseBoard &board,std::vector<BoardCoordinates>& moves)
{
    // this is only for the queen
    // the rook and the bishop
    uint64_t attackable_squares = board.white_to_move ? board.black_pieces : board.white_pieces;
    uint64_t not_attackable_squares = board.white_to_move ? board.white_pieces : board.black_pieces;
    for (auto move : m_possible_moves[piece])
    { // i first create the vector

        for (int y = origin.y; y < 8 && y >= 0; y += move.y)
        {
            for (int x = origin.x; x < 8 && x >= 0; x += move.x)
            {

                uint64_t mask = 1ULL << ((y * 8) + x);
                if (not_attackable_squares & mask)
                {
                    goto finished_line;
                }

                BoardCoordinates new_move{
                    .x = x,
                    .y = y,
                };
                moves.push_back(new_move);
                if (attackable_squares & mask)
                {
                    goto finished_line;
                }
            }
        }
    finished_line:
    }

}