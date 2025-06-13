#include "board.h++"

bool Board::EnemySquares(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t enemy_mask = board.white_to_move ? board.black_pieces : board.white_pieces;
    return (piece_mask & enemy_mask);
}
bool Board::FriendSquares(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t friend_mask = board.white_to_move ? board.white_pieces : board.black_pieces;
    return (piece_mask & friend_mask);
}

bool Board::OcuppiedSquares(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    return (piece_mask & GetUtilizedSquares(board)) ;
}
TypePiece Board::GetPieceFromCoord(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    bool isWhite = (board.white_pieces & piece_mask) ;
    // PAWNS
    if (piece_mask & board.pawns)
    {
        return {Pieces::PAWN, isWhite};
    }
    // KNIGHTS
    else if (piece_mask & board.knights)
    {
        return {Pieces::KNIGHT, isWhite};
    }
    // BISHOPS
    else if (piece_mask & board.bishops)
    {
        return {Pieces::BISHOP, isWhite};
    }
    // ROOKS
    else if (piece_mask & board.rooks)
    {
        return {Pieces::ROOK, isWhite};
    }
    // QUEEN
    else if (piece_mask & board.queens)
    {
        return {Pieces::QUEEN, isWhite};
    }
    // KING
    else if (piece_mask & board.kings)
    {
        return {Pieces::KING, isWhite};
    }
    return {Pieces::NONE, isWhite};
}

uint64_t Board::GetUtilizedSquares(const BitWiseBoard &board)
{
    return board.white_pieces | board.black_pieces;
}
