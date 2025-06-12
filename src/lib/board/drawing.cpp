#include "board.h++"
#include <iostream>
#include <bitset>

std::array<TypePiece, 64> Board::GetPieces(BitWiseBoard &board)
{
    bool once = false;
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
            if (once)
                std::cout << "PAWN " << std::bitset<64>(board.pawns) << std::endl;
            pieces[i] = {Pieces::PAWN, is_white};
        }
        else if (board.knights & mask)
        {
            if (once)
                std::cout << "KNIGHT " << std::bitset<64>(board.knights) << std::endl;
            pieces[i] = {Pieces::KNIGHT, is_white};
        }
        else if (board.bishops & mask)
        {
            if (once)
                std::cout << "BISHOP " << std::bitset<64>(board.bishops) << std::endl;
            pieces[i] = {Pieces::BISHOP, is_white};
        }
        else if (board.rooks & mask)
        {
            if (once)
                std::cout << "ROOK " << std::bitset<64>(board.rooks) << std::endl;
            pieces[i] = {Pieces::ROOK, is_white};
        }
        else if (board.queens & mask)
        {
            if (once)
                std::cout << "QUEEN " << std::bitset<64>(board.queens) << std::endl;
            pieces[i] = {Pieces::QUEEN, is_white};
        }
        else if (board.kings & mask)
        {
            if (once)
                std::cout << "KING " << std::bitset<64>(board.kings) << std::endl;
            pieces[i] = {Pieces::KING, is_white};
        }else{
            uint64_t white_board=board.white_pieces;
            uint64_t black_board=board.black_pieces;
            if(black_board&mask){
                pieces[i]={Pieces::UNKOWN,false};
            }else if(white_board&mask){
                pieces[i]={Pieces::UNKOWN,true};
            }
        }
    }
    once = false;
    return pieces;
}