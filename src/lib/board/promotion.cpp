#include "board-api.h++"
#define BLACK_SIDE 0xff00000000000000
#define WHITE_SIDE 0x00000000000000ff
BitWiseBoard BoardAPI::Promotion(BoardCoordinates from, const BitWiseBoard &board, Pieces new_piece, std::optional<bool> is_white)
{
    const TypePiece piece = GetPieceFromCoord(from, board);
    if (piece.piece != Pieces::PAWN)
    {
        return board;
    }
    const bool white_to_move = is_white.value_or(board.white_to_move);
    const uint64_t position_mask = (1ULL) << ((from.y * 8) + from.x);
    const uint64_t finish_mask = white_to_move ? WHITE_SIDE : BLACK_SIDE;

    if (!(finish_mask & position_mask))
    {
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
    new_board.white_to_move = !white_to_move;

    new_board.attacked_squares = getAttackedSquares(new_board);
    new_board.zobrist = GetZobrist(new_board);
    new_board.potenital_attacks = getPotentialAttacks(new_board);
    uint64_t new_friend_mask = new_board.white_to_move ? board.white_pieces : board.black_pieces;

    // here we are going to calculate the squares that we could attack :)
    // okay here goes some basic shit :)

    new_board.king_check = board.kings & new_friend_mask & new_board.attacked_squares;
    //
    return new_board;
}
