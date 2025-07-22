#include "board-api.h++"
#include <iostream>
#include <bitset>

std::array<TypePiece, 64> BoardAPI::GetPieces(BitWiseBoard &board)
{
    std::array<TypePiece, 64> pieces;
    for (int i = 0; i < 64; i++)
    {
        pieces[i] = {Pieces::NONE, false};
    }
    // now we need to check the pieces
    for (int i = 0; i < 64; i++)
    {
        uint64_t mask = 1ULL << i;

        bool is_white = (bool)(board.white_pieces & mask);

        if (board.pawns & (mask))
        {
            pieces[i] = {Pieces::PAWN, is_white};
        }
        else if (board.knights & mask)
        {
            pieces[i] = {Pieces::KNIGHT, is_white};
        }
        else if (board.bishops & mask)
        {
            pieces[i] = {Pieces::BISHOP, is_white};
        }
        else if (board.rooks & mask)
        {
            pieces[i] = {Pieces::ROOK, is_white};
        }
        else if (board.queens & mask)
        {
            pieces[i] = {Pieces::QUEEN, is_white};
        }
        else if (board.kings & mask)
        {
            pieces[i] = {Pieces::KING, is_white};
        }
        else
        {
            uint64_t white_board = board.white_pieces;
            uint64_t black_board = board.black_pieces;
            if (black_board & mask)
            {
                pieces[i] = {Pieces::UNKOWN, false};
            }
            else if (white_board & mask)
            {
                pieces[i] = {Pieces::UNKOWN, true};
            }
        }
    }
    std::cout<<"enpassant: "<<std::bitset<64>(board.enpassant)<<"\n";

    return pieces;
}