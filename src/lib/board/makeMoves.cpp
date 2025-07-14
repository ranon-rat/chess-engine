#include "board.h++"
#include <cmath>

BitWiseBoard Board::MakeMove(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board, bool simulation)
{
    BitWiseBoard new_board = board;
    if (from.x == to.x && from.y == to.y)
    {
        return new_board;
    }

    // First i need to check if the piece is a legal move,
    // thats all :) XD

    // if we are not part of the utilized squares, or the piece is part of the attackable squares then we shouldnt continue :)
    if (!FriendSquares(from, board, board.white_to_move) || EnemySquares(from, board, board.white_to_move))
    { // if its not in one of our pieces then just return 0 :)
        return new_board;
    }
    // mask of the targets and pieces :)
    uint64_t piece_mask = 1ULL << ((from.y * 8) + from.x);
    uint64_t target_mask = 1ULL << ((to.y * 8) + to.x);
    // en passant part to reset everything
    if (!simulation) // if its a simulation we dont have to emulate any of this shit
    {
        MaxMovesArray legal_moves = GetMoves(from, board, board.white_to_move);
        // SO THIS IS JUT TO FILTER ANY KIND OF BULLSHIT SO I DONT HAVE TO LOSE ANY TIME CALCULATING SHIT :)
        bool is_legal = false;
        for (size_t i = 0; i < legal_moves.size(); i++)
        {
            BoardCoordinates move = legal_moves[i];
            if (move.x == to.x && move.y == to.y)
            {
                is_legal = true;
                break;
            }
        }
        if (!is_legal)
        {
            return new_board;
        }
    }
    //
    new_board.enpassant = initialize_board; // we reset this before we have to alter it :);
    int direction = board.white_to_move ? -1 : 1;

    TypePiece origin_piece = GetPieceFromCoord(from, board);
    switch (origin_piece.piece) // this will write the pieces position :)
    {
    case Pieces::PAWN:
        movePawn(from, to, new_board, board, piece_mask, target_mask, direction);
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
        moveRook(from, to, new_board, board, piece_mask, target_mask);
        break;
    case Pieces::QUEEN:
        new_board.queens &= ~piece_mask;
        new_board.queens |= target_mask;
        break;
    case Pieces::KING:

        moveKing(from, to, new_board, board, piece_mask, target_mask);
        break;
    default:
        break;
    }
    /*this is only for the enpassant case :)*/
    TypePiece target_piece = GetPieceFromCoord(to, board);
    if ((EnemySquares(to, board, board.white_to_move) || target_piece.piece == Pieces::NONE) && target_piece.piece != origin_piece.piece)
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
            eatPawnEnPassant(from, to, new_board, board, piece_mask, target_mask, direction);
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
        if (origin_piece.piece == Pieces::KING)
        {
            new_board.white_can_castle_kingside = false;
            new_board.white_can_castle_queenside = false;
        }
    }
    else
    {
        new_board.black_pieces &= ~piece_mask;
        new_board.black_pieces |= target_mask;
        new_board.white_pieces &= ~target_mask;
        if (origin_piece.piece == Pieces::KING)
        {
            new_board.black_can_castle_kingside = false;
            new_board.black_can_castle_queenside = false;
        }
    }
    if (!IsReadyToPromote(new_board))
    {
        new_board.white_to_move = !board.white_to_move;

        if (!simulation)
        {
            new_board.attacked_squares = getAttackedSquares(new_board);
            // so we need to first define the enemy mask
            uint64_t enemy_mask = board.white_to_move ? board.black_pieces : board.white_pieces;

            // here we are going to calculate the squares that we could attack :)
            // okay here goes some basic shit :)

            new_board.king_check = board.kings & enemy_mask & new_board.attacked_squares;
        }
    }
    return new_board;
}

void Board::moveRook(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask)
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

void Board::movePawn(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int8_t direction)
{
    new_board.pawns &= ~initial_mask;
    new_board.pawns |= target_mask;
    // once you make a move you lost the right to make a move in that specific part :)
    // this is only for the en passant :)
    if (abs(from.y - to.y) == 2)
    {
        uint64_t enpassant_mask = 1ULL << ((from.y + direction) * 8 + from.x);
        new_board.enpassant |= enpassant_mask;
    }
}

void Board::eatPawnEnPassant(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask, int8_t direction)
{
    const TypePiece origin_piece = GetPieceFromCoord(from, board);
    if (origin_piece.piece != Pieces::PAWN)
    {
        return;
    }
    BoardCoordinates enemy_coords = {
        .x = to.x,
        .y = static_cast<int8_t>(to.y - direction),
    };

    const TypePiece enemy_piece = GetPieceFromCoord(enemy_coords, board);
    if (enemy_piece.piece != PAWN || !EnemySquares(enemy_coords, board, board.white_to_move))
    {
        return;
    }

    uint64_t enemy_pawn_mask = 1ULL << ((enemy_coords.y * 8) + enemy_coords.x);

    new_board.pawns &= ~enemy_pawn_mask;
    if (board.white_to_move)
    {
        new_board.black_pieces &= ~enemy_pawn_mask;
    }
    else
    {
        new_board.white_pieces &= ~enemy_pawn_mask;
    }
    return;
}

void Board::moveKing(BoardCoordinates from, BoardCoordinates to, BitWiseBoard &new_board, const BitWiseBoard &board, uint64_t initial_mask, uint64_t target_mask)
{

    // this is just something quite basic
    new_board.kings &= ~initial_mask;
    new_board.kings |= target_mask;
    if (board.white_to_move)
    {
        new_board.white_can_castle_kingside = false;
        new_board.white_can_castle_queenside = false;
    }
    else
    {
        new_board.black_can_castle_kingside = false;
        new_board.black_can_castle_queenside = false;
    }

    if (abs(from.x - to.x) == 1)
    {
        return;
    }
    bool king_side = std::signbit(from.x - to.x);
    int x = 7 * king_side;
    uint64_t origin_rook_mask = (1ULL << ((from.y * 8) + x));
    uint64_t new_rook_mask(1ULL << ((from.y * 8) + from.x + (1 - (!king_side) * 2)));
    new_board.rooks &= ~origin_rook_mask;
    new_board.rooks |= new_rook_mask;
    // so i need to erase now :)
    if (board.white_to_move)
    {
        new_board.white_pieces &= ~origin_rook_mask;
        new_board.white_pieces |= new_rook_mask;
    }
    else
    {
        new_board.black_pieces &= ~origin_rook_mask;
        new_board.black_pieces |= new_rook_mask;
    }
}