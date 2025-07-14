#include "board.h++"
#define BLACK_SIDE 0xff00000000000000
#define WHITE_SIDE 0x00000000000000ff
#include <iostream>
#include <bitset>
BitWiseBoard Board::Promotion(BoardCoordinates from, const BitWiseBoard &board, Pieces new_piece, std::optional<bool> is_white)
{
    const TypePiece piece = GetPieceFromCoord(from, board);
    if (piece.piece != Pieces::PAWN)
    {
        std::cout<<"not valid piece\n";
        return board;
    }
    const bool white_to_move = is_white.value_or(board.white_to_move);
    const uint64_t position_mask = (1ULL) << ((from.y * 8) + from.x);
    const uint64_t finish_mask = white_to_move ? WHITE_SIDE : BLACK_SIDE;

    std::cout <<"IS WHITE? "<<(white_to_move?"true":"false")<<"\n";
    if (!(finish_mask & position_mask))
    {   
        std::cout<<"IS WHITE BUT NOT VALID COLOR :/ "<<std::bitset<64> ( position_mask) <<std::bitset<64> (finish_mask & position_mask)<<"\n";
        return board;
    }
    BitWiseBoard new_board = board;

    new_board.pawns &= ~position_mask;
    

    switch (new_piece)
    {
    case QUEEN:
        new_board.queens |= position_mask;
        break;
    case ROOK:
        new_board.rooks |= position_mask;
        break;
    case BISHOP:
        new_board.bishops |= position_mask;
        break;
    case KNIGHT:
        new_board.knights |= position_mask;
        break;
    default:
        break;
    }
    new_board.attacked_squares=GetAttackedSquares(new_board);
    new_board.white_to_move=!white_to_move;
    return new_board;
}
