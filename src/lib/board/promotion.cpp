#include "board-api.h++"
#define BLACK_SIDE 0xff00000000000000
#define WHITE_SIDE 0x00000000000000ff
#define PRE_PROMOTION_LINE 0x00ff00000000ff00
#define PROMOTION_LINE 0xff000000000000ff
// this is for the bot
bool BoardAPI::CanPromote(const BoardCoordinates &from, const BoardCoordinates &to, const BitWiseBoard &board)
{
    const uint64_t from_mask = (1ULL) << (from.y * 8 + from.x);
    const uint64_t to_mask = (1ULL) << (to.y * 8 + to.x);
    return (PRE_PROMOTION_LINE & from_mask & board.pawns) && (to_mask & PROMOTION_LINE);
}

void BoardAPI::Promotion(BoardCoordinates from, BitWiseBoard &board, Pieces new_piece)
{
    const TypePiece piece = GetPieceFromCoord(from, board);
    if (piece.piece != Pieces::PAWN)
    {
        return;
    }
    const uint64_t position_mask = (1ULL) << ((from.y * 8) + from.x);
    const uint64_t finish_mask = board.white_to_move ? WHITE_SIDE : BLACK_SIDE;

    if (!(finish_mask & position_mask))
    {
        return;
    }

    board.pawns &= ~position_mask;

    switch (new_piece)
    {
    case QUEEN:
        board.queens |= position_mask;
        break;
    case ROOK:
        board.rooks |= position_mask;
        break;
    case BISHOP:
        board.bishops |= position_mask;
        break;
    case KNIGHT:
        board.knights |= position_mask;
        break;
    default:
        break;
    }
    board.white_to_move = !board.white_to_move;

    board.attacked_squares = getAttackedSquares(board);
    board.zobrist = GetZobrist(board);
    board.potenital_attacks = getPotentialAttacks(board);
    uint64_t new_friend_mask = board.white_to_move ? board.white_pieces : board.black_pieces;

    // here we are going to calculate the squares that we could attack :)
    // okay here goes some basic shit :)

    board.king_check = board.kings & new_friend_mask & board.attacked_squares;
    //
    return;
}
