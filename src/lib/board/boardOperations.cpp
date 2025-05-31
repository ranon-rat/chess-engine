#include "board.h++"
#include <iostream>
#include <bitset>
#include <cmath>

bool Board::AttackableSquares(BoardCoordinates from, const BitWiseBoard &board)
{
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t enemy_mask = board.white_to_move ? board.black_pieces : board.white_pieces;
    return (piece_mask & enemy_mask) > 0;
}
bool Board::FriendSquares(BoardCoordinates from, const BitWiseBoard &board)
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
    if (from.x == to.x && from.y == to.y)
    {
        return new_board;
    }

    // First i need to check if the piece is a legal move,
    // thats all :) XD

    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t target_mask = 1ULL << ((to.y * 8) + to.x);
    // now we have to check the kinds
    uint64_t occupied_squares = board.white_to_move ? board.white_pieces : board.black_pieces;
    // if we are not part of the utilized squares, or the piece is part of the attackable squares then we shouldnt continue :)
    if (!FriendSquares(from, board) || AttackableSquares(from, board))
    { // if its not in one of our pieces then just return 0 :)
        std::cout << "this isnt your piece" << "\n";
        return new_board;
    }
    new_board.enpassant = initialize_board; // we reset this before we have to alter it :);
    TypePiece origin_piece = GetPieceFromCoord(from, board);
    switch (origin_piece.piece) // this will write the pieces position :)
    {
    case Pieces::PAWN:
        new_board.pawns &= ~piece_mask;
        new_board.pawns |= target_mask;
        new_board.right_long_move &= ~piece_mask;
        if(abs(from.y-to.y)==2){
            std::cout<<"EN PASSANT\n";
            uint64_t enpassant_mask=1ULL<<(from.y+1)+from.x;
            new_board.enpassant|=enpassant_mask;
        }
        // new_board.enpassant;// do something here
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

    if (AttackableSquares(to, board) && target_piece.piece != origin_piece.piece)
    {

        switch (target_piece.piece)
        {
        case Pieces::PAWN:
            new_board.pawns &= ~target_mask;
            new_board.right_long_move &= ~target_mask;

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
void Board::AlterLongMove(BoardCoordinates origin, BitWiseBoard &board)
{
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
    if (!(FriendSquares(piece, board)))
    {
        return moves;
    }

    // avaible pieces
    // uint64_t avaible_pieces=0;
    TypePiece piece_info = GetPieceFromCoord(piece, board);
    switch (piece_info.piece)
    {
    case Pieces::PAWN:
        break;
    case Pieces::KNIGHT:
        OneLineMoves(KNIGHT, piece, board, moves);
        break;
    case Pieces::BISHOP:
        LineMoves(BISHOP, piece, board, moves);
        break;
    case Pieces::ROOK:
        LineMoves(ROOK, piece, board, moves);

        break;
    case Pieces::QUEEN:
        LineMoves(QUEEN, piece, board, moves);

        break;
    case Pieces::KING:
        OneLineMoves(KING, piece, board, moves);

        break;
    default:
        break;
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
void Board::LineMoves(Pieces piece, BoardCoordinates origin, BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter)
{
    // this is only for the queen
    // the rook and the bishop
    for (auto move : m_possible_moves[piece])
    { // i first create the vector

        for (int y = origin.y; y < 8 && y >= 0; y += move.y)
        {
            for (int x = origin.x; x < 8 && x >= 0; x += move.x)
            {

                BoardCoordinates coords = {
                    .x = x,
                    .y = y,
                };
                if (FriendSquares(coords, board))
                {
                    switch (filter)
                    {
                    case Defendable:
                        moves.emplace_back(coords);
                    case Legal:
                        goto finished_line;
                        break;
                    default:
                        break;
                    }
                }

                moves.emplace_back(coords);
                if (AttackableSquares(coords, board) && (filter == Legal))
                {
                    goto finished_line;
                }
            }
        }
    finished_line:
    }
}

void Board::OneLineMoves(Pieces piece, BoardCoordinates origin, BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter)
{

    // this is only for the queen
    // the rook and the bishop
    for (auto move : m_possible_moves[piece])
    { // i first create the vector
        const int new_x = origin.x + move.x;
        const int new_y = origin.y + move.y;
        if (new_x < 0 || new_x > 8 || new_y < 0 || new_y > 8)
        {
            continue;
        }
        BoardCoordinates coords = {
            .x = new_x,
            .y = new_y,
        };
        if (FriendSquares(coords, board) && (filter == Legal))
        {
            continue;
        }

        moves.emplace_back(coords);
    }
}

void Board::PawnMove(BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter)
{
    int direction = board.white_to_move ? -1 : 1; // this is important :)
    uint64_t pawn_mask = 1ULL << ((origin.y * 8) + origin.x);
    bool long_move_right = (pawn_mask & board.right_long_move) > 0;
    int advance=long_move_right?2:1;
    // attack position :)
    for (int i = -1; i <= 1; i += 2)
    {
        const BoardCoordinates new_coords = {
            .x = origin.x + i,
            .y = origin.y + direction};

        const uint64_t attack_mask = board.enpassant & 1ULL << ((new_coords.y * 8) + new_coords.x);
        const bool enpassant = (board.enpassant & attack_mask) > 0;

        if (!AttackableSquares(new_coords, board) && !enpassant && filter == TypeFilter::Legal)
        {
            continue;
        }
        moves.emplace_back(new_coords);
    }

    // now advance position
    for (int i = 1; i <= advance; i++)
    {
        const BoardCoordinates new_coords = {
            .x = origin.x,
            .y = origin.y + i * direction,
        };
        if (AttackableSquares(new_coords, board) || FriendSquares(new_coords, board))
        {
            break;
        }
        moves.emplace_back(new_coords);
    }
}
