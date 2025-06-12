#include "board.h++"
#include <iostream>
#include <bitset>
#include <cmath>

BitWiseBoard Board::MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board)
{
    BitWiseBoard new_board = board;
    if (from.x == to.x && from.y == to.y)
    {
        return new_board;
    }

    // First i need to check if the piece is a legal move,
    // thats all :) XD

    // now we have to check the kinds
    uint64_t occupied_squares = board.white_to_move ? board.white_pieces : board.black_pieces;
    // if we are not part of the utilized squares, or the piece is part of the attackable squares then we shouldnt continue :)
    if (!FriendSquares(from, board) || EnemySquares(from, board))
    { // if its not in one of our pieces then just return 0 :)
        std::cout << "this isnt your piece" << "\n";
        return new_board;
    }
    // mask of the targets and pieces :)
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t target_mask = 1ULL << ((to.y * 8) + to.x);
    // en passant part to reset everything
    std::vector<BoardCoordinates> legal_moves=GetMoves(from,board);
    // SO THIS IS JUT TO FILTER ANY KIND OF BULLSHIT SO I DONT HAVE TO LOSE ANY TIME CALCULATING SHIT :)
    bool is_legal=false;
    for(BoardCoordinates move:legal_moves){
        if(move.x==to.x&&move.y==to.y){
            is_legal=true;
            break;
        }
    }
    if(!is_legal){
        return new_board;
    }
    //
    new_board.enpassant = initialize_board; // we reset this before we have to alter it :);
    int direction = board.white_to_move ? -1 : 1;

    TypePiece origin_piece = GetPieceFromCoord(from, board);
    switch (origin_piece.piece) // this will write the pieces position :)
    {
    case Pieces::PAWN:
        MovePawn(from, to, new_board, board, piece_mask, target_mask, direction);
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
        MoveRook(from, to, new_board, board, piece_mask, target_mask);
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
    /*this is only for the enpassant case :)*/
    TypePiece target_piece = GetPieceFromCoord(to, board);
    uint64_t enemy_pawn_mask = 0;
    TypePiece enemy_piece = {.piece = NONE, .isWhite = false};
    BoardCoordinates enemy_coords;

    if ((EnemySquares(to, board) || target_piece.piece == Pieces::NONE) && target_piece.piece != origin_piece.piece)
    {

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
            if (to.x == 8)
            {
                if (board.white_to_move)
                {
                    new_board.black_can_castle_kingside = false;
                }
                else
                {
                    new_board.white_can_castle_kingside = false;
                }
            }
            else if (to.x == 0)
            {
                if (board.white_to_move)
                {
                    new_board.black_can_castle_queenside = false;
                }
                else
                {
                    new_board.white_can_castle_queenside = false;
                }
            }
            new_board.rooks &= ~target_mask;

            break;
        case Pieces::QUEEN:
            new_board.queens &= ~target_mask;
            break;
        case Pieces::KING:
            new_board.kings &= ~target_mask;
            break;
        case Pieces::NONE:
           EatPawnEnPassant(from,to,new_board,board,piece_mask,target_mask,direction);
            break;

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

    new_board.white_to_move =!board.white_to_move;

    return new_board;
}

void Board::MoveRook(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask)
{
    // queen side
    if (to.x == 8)
    {
        if (board.white_to_move)
        {
            new_board.white_can_castle_kingside = false;
        }
        else
        {
            new_board.black_can_castle_kingside = false;
        }
    } // king side
    else if (to.x == 0)
    {
        if (board.white_to_move)
        {
            new_board.white_can_castle_queenside = false;
        }
        else
        {
            new_board.black_can_castle_queenside = false;
        }
    }
    new_board.rooks &= ~initial_mask;
    new_board.rooks |= target_mask;
}

void Board::MovePawn(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int direction)
{
    new_board.pawns &= ~initial_mask;
    new_board.pawns |= target_mask;
    // once you make a move you lost the right to make a move in that specific part :)
    // this is only for the en passant :)
    if (abs(from.y - to.y) == 2)
    {
        std::cout << "EN PASSANT\n";
        uint64_t enpassant_mask = 1ULL << ((from.y + direction) * 8 + from.x);
        std::cout << std::bitset<64>(enpassant_mask) << "\n";
        new_board.enpassant |= enpassant_mask;
    }
}

void Board::EatPawnEnPassant(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int direction)
{
    const TypePiece origin_piece=GetPieceFromCoord(from,board);
    if (origin_piece.piece != Pieces::PAWN)
    {
        return;
    }
    std::cout << "something interesting\n";
    BoardCoordinates enemy_coords = {
        .x = to.x,
        .y = to.y - direction,
    };

    const TypePiece enemy_piece = GetPieceFromCoord(enemy_coords, board);
    if (enemy_piece.piece != PAWN || !EnemySquares(enemy_coords, board))
    {
        return;
    }
    
    uint64_t enemy_pawn_mask = 1ULL << ((enemy_coords.y * 8) + enemy_coords.x);
    
    new_board.pawns &= ~enemy_pawn_mask;
}
