#include "board.h++"
#include <iostream>
#include <bitset>

bool Board::AttackableSquares(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t enemy_mask = board.white_to_move ? board.black_pieces : board.white_pieces;
    return (piece_mask & enemy_mask) > 0;
}
bool Board::UtilizedSquares(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t friend_mask = board.white_to_move ? board.white_pieces : board.black_pieces;
    return (piece_mask & friend_mask) > 0;
}

TypePiece Board::GetPieceFromCoord(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    bool isWhite = (board.white_pieces & piece_mask) > 0;
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
BitWiseBoard Board::MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board)
{
    BitWiseBoard new_board = board;
    if(from.x==to.x&&from.y==to.y){
        return new_board;
    }
    // First i need to check if the piece is a legal move,
    // thats all :) XD

    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t target_mask = 1ULL << ((to.y * 8) + to.x);
    // now we have to check the kinds
    uint64_t occupied_squares = board.white_to_move ? board.white_pieces : board.black_pieces;
    // if we are not part of the utilized squares, or the piece is part of the attackable squares then we shouldnt continue :)
    if (!UtilizedSquares(from, board) || AttackableSquares(from, board))
    { // if its not in one of our pieces then just return 0 :)
        std::cout << "this isnt your piece" << "\n";
        return new_board;
    }
    TypePiece origin_piece = GetPieceFromCoord(from, board);
    switch (origin_piece.piece) // this will write the pieces position :)
    {
    case Pieces::PAWN:
        new_board.pawns &= ~piece_mask;
        new_board.pawns |= target_mask;
        break;
    case Pieces::KNIGHT:
        new_board.knights &= ~piece_mask;
        new_board.knights |= target_mask;
        break;
    case Pieces::BISHOP:
        new_board.bishops &= ~piece_mask;
        new_board.bishops |= target_mask;
        break;
    case Pieces::ROOK:
        new_board.rooks &= ~piece_mask;
        new_board.rooks |= target_mask;
        break;
    case Pieces::QUEEN:
        new_board.queens &= ~piece_mask;
        new_board.queens |= target_mask;
        break;
    case Pieces::KING:
        new_board.kings &= ~piece_mask;
        new_board.kings |= target_mask;
        break;
    default:
        break;
    }
    TypePiece target_piece = GetPieceFromCoord(to, board);
    std::cout << "it seems that we are getting the correct value right?\n"
                  << target_piece.piece << " " << origin_piece.piece << "\n";

    if (AttackableSquares(to, board) && target_piece.piece != origin_piece.piece)
    {
        std::cout << "it seems that we are getting the correct value right?\n"
                  << target_piece.piece << " " << origin_piece.piece << "\n";

        switch (target_piece.piece)
        {
        case Pieces::PAWN:
            new_board.pawns &= ~target_mask;
            break;
        case Pieces::KNIGHT:
            new_board.knights &= ~target_mask;
            break;
        case Pieces::BISHOP:
            new_board.bishops &= ~target_mask;
            break;
        case Pieces::ROOK:
            new_board.rooks &= ~target_mask;

            break;
        case Pieces::QUEEN:
            new_board.queens &= ~target_mask;
            break;
        case Pieces::KING:
            new_board.kings &= ~target_mask;

        default:
            break;
        }
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
        LineMoves(BISHOP, piece, board, moves);
    }
    else if (piece_mask & board.rooks)
    {
        LineMoves(ROOK, piece, board, moves);
    }
    else if (piece_mask & board.queens)
    {
        LineMoves(QUEEN, piece, board, moves);
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
void Board::LineMoves(Pieces piece, BoardCoordinates origin, BitWiseBoard &board, std::vector<BoardCoordinates> &moves)
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