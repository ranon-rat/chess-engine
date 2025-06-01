#include "board.h++"
#include <iostream>
std::vector<BoardCoordinates> Board::GetMoves(BoardCoordinates piece, BitWiseBoard &board, TypeFilter filter)
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
        PawnMoves(piece, board, moves, filter);
        break;
    case Pieces::KNIGHT:
        OneLineMoves(KNIGHT, piece, board, moves, filter);
        break;
    case Pieces::BISHOP:
        LineMoves(BISHOP, piece, board, moves, filter);
        break;
    case Pieces::ROOK:
        LineMoves(ROOK, piece, board, moves, filter);
        break;
    case Pieces::QUEEN:
        LineMoves(QUEEN, piece, board, moves, filter);
        break;
    case Pieces::KING:
        OneLineMoves(KING, piece, board, moves, filter);
        CastlingMoves(piece, board, moves);
        break;
    default:
        break;
    }

    return moves;
}

void Board::LineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter)
{
    // this is only for the queen
    // the rook and the bishop
    for (auto move : m_possible_moves[piece])
    { // i first create the vector
        std::cout<<"move :)"<<"\n";
        for (int y = origin.y+move.y; y < 8 && y >= 0; y += move.y)
        {
            for (int x = origin.x+move.x; x < 8 && x >= 0; x += move.x)
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
                if (EnemySquares(coords, board) && (filter == Legal))
                {
                    goto finished_line;
                }
            }
        }
    finished_line:
    }
}

void Board::OneLineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter)
{

    // this is only for the queen
    // the rook and the bishop
    for (auto move : m_possible_moves[piece])
    { // i first create the vector
        const int new_x = origin.x + move.x;
        const int new_y = origin.y + move.y;
        if (new_x < 0 || new_x >= 8 || new_y < 0 || new_y >= 8)
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

void Board::PawnMoves(BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves, TypeFilter filter)
{
    int direction = board.white_to_move ? -1 : 1; // this is important :)
    uint64_t pawn_mask = 1ULL << ((origin.y * 8) + origin.x);
    bool long_move_right = (pawn_mask & board.right_long_move) > 0;
    int advance = long_move_right ? 2 : 1;
    // attack position :)
    for (int i = -1; i <= 1; i += 2)
    {
        const BoardCoordinates new_coords = {
            .x = origin.x + i,
            .y = origin.y + direction};

        const uint64_t attack_mask = board.enpassant & 1ULL << ((new_coords.y * 8) + new_coords.x);
        const bool enpassant = (board.enpassant & attack_mask) > 0;

        if (!EnemySquares(new_coords, board) && !enpassant && filter == TypeFilter::Legal)
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
        if (EnemySquares(new_coords, board) || FriendSquares(new_coords, board))
        {
            break;
        }
        moves.emplace_back(new_coords);
    }
}

void Board::CastlingMoves(BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves)
{
    bool can_castle = board.white_to_move ? board.white_can_castle : board.black_can_castle;
    if (!can_castle)
    {
        return;
    }
    // i should check first the
    // i should check first the queen side or what? :///
    struct CastlingRight
    {
        int dx;
        bool castling_right;
    };
    std::array<CastlingRight, 2> castling = {
        CastlingRight{

            .dx = 1,
            .castling_right = board.white_to_move ? board.white_can_castle_kingside : board.black_can_castle_kingside,
        },
        CastlingRight{
            .dx = (-1),
            .castling_right = board.white_to_move ? board.white_can_castle_queenside : board.black_can_castle_queenside,

        },
    };
    for (CastlingRight &v : castling)
    {
        if (!v.castling_right)
        {
            continue;
        }
        // 0 check 1 not check but the line is attacked 2 the destiny is attacked, you will be on check :)
        for (int i = 0; i <= 2; i++)
        {
            int new_x = origin.x + v.dx * i;
            uint64_t mask = 1ULL << ((origin.y * 8) + new_x);
            BoardCoordinates new_coords = {
                .x = new_x,
                .y = origin.y,
            };
            if ((board.attacked_squares & mask) > 0 || OcuppiedSquares(new_coords, board))
            {
                break;
            }
            if (i != 2)
            {
                continue;
            }
            moves.emplace_back(new_coords);
        }
    }
}
