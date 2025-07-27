#include "board-api.h++"
MaxMovesArray BoardAPI::GetMoves(BoardCoordinates piece, const BitWiseBoard &board, TypeFilter filter, std::optional<bool> is_white)
{
    // checking to//
    // now we need to check if the piece is a pawn or not

    bool from_white = (is_white.value_or(board.white_to_move));
    // uint64_t attack_mask=getAttackedSquares(board,from_white);

    MaxMovesArray moves;
    // hmmm
    // first lets check if the piece is one of us
    if (!(FriendSquares(piece, board, from_white)))
    {
        return moves;
    }

    // avaible pieces
    // uint64_t avaible_pieces=0;
    TypePiece piece_info = GetPieceFromCoord(piece, board);
    switch (piece_info.piece)
    {
    case Pieces::PAWN:
        pawnMoves(piece, board, moves, from_white, filter);
        break;
    case Pieces::KNIGHT:
        oneLineMoves(KNIGHT, piece, board, moves, from_white, filter);
        break;
    case Pieces::BISHOP:
        lineMoves(BISHOP, piece, board, moves, from_white, filter);
        break;
    case Pieces::ROOK:
        lineMoves(ROOK, piece, board, moves, from_white, filter);
        break;
    case Pieces::QUEEN:
        lineMoves(QUEEN, piece, board, moves, from_white, filter);
        break;
    case Pieces::KING:
        oneLineMoves(KING, piece, board, moves, from_white, filter);
        castlingMoves(piece, board, moves, from_white, board.attacked_squares, filter);
        break;
    default:
        break;
    }

    if (filter != Legal)
    {
        return moves;
    }
    // i just need to check a really simple thing
    uint64_t king_mask = (board.white_to_move ? board.white_pieces : board.black_pieces) & board.kings;
    uint64_t piece_mask = (1ULL) << (piece.y * 8 + piece.x);
    MaxMovesArray filtered_moves = {};

    // i check if the piece in on the piece_mask
    if (!(piece_mask & board.potenital_attacks) && !(king_mask & board.attacked_squares))
    {
        if (piece_info.piece != KING)
            return moves;

        for (size_t i = 0; i < moves.size(); i++)
        {
            BoardCoordinates &move = moves[i];
            uint64_t piece_mask = (1ull) << (move.y * 8 + move.x);
            if (piece_mask & board.attacked_squares)
                continue;
            filtered_moves.emplace_back(move);
        }
        return filtered_moves;
    }
    for (size_t i = 0; i < moves.size(); i++)
    {
        BoardCoordinates &move = moves[i];
        if (IsChecked(piece, move, board, from_white))
            continue;
        filtered_moves.emplace_back(move);
    }

    return filtered_moves;
}

void BoardAPI::lineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter)
{
    // this is only for the queen
    // the rook and the bishop
    for (auto move : m_possible_moves[piece])
    { // i first create the vector
        int max_how_deep = filter == Line ? 2 : 1;

        int found_friend = 0;
        int found_enemy = 0;
        for (int8_t x = origin.x + move.x,
                    y = origin.y + move.y;
             x < 8 && x >= 0 &&
             y < 8 && y >= 0;
             x += move.x, y += move.y)
        {

            BoardCoordinates coords = {
                .x = x,
                .y = y,
            };

            if (FriendSquares(coords, board, is_white))
            {

                if (filter != Legal)
                {
                    found_friend++;
                    moves.emplace_back(coords);
                    if (found_friend >= max_how_deep)
                        break;
                    continue;
                }
                if (filter == Legal)
                    break;
            }

            moves.emplace_back(coords);
            if (EnemySquares(coords, board, is_white))
            {
                found_enemy++;
                if (found_enemy >= max_how_deep)
                    break;
            }
        }
    }
}
void BoardAPI::oneLineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter)
{
    if (filter == TypeFilter::Line)
        return;

    // this is only for the queen
    // the rook and the bishop

    for (auto move : m_possible_moves[piece])
    { // i first create the vector
        const int8_t new_x = origin.x + move.x;
        const int8_t new_y = origin.y + move.y;
        if (new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7)
            continue;
        BoardCoordinates coords = {
            .x = new_x,
            .y = new_y,
        };
        if ((filter == Legal) && FriendSquares(coords, board, is_white))
            continue;

        moves.emplace_back(coords);
    }
}

void BoardAPI::pawnMoves(BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, TypeFilter filter)
{

    int8_t direction = is_white ? -1 : 1; // this is important :)
    uint64_t pawn_mask = 1ULL << ((origin.y * 8) + origin.x);
    bool long_move_right = (pawn_mask & pawn_positions) > 0;
    size_t advance = long_move_right ? 2 : 1;

    // attack position :)
    for (int8_t i = -1; i <= 1; i += 2)
    {
        const BoardCoordinates new_coords = {
            .x = static_cast<int8_t>(origin.x + i),
            .y = static_cast<int8_t>(origin.y + direction)};

        if (new_coords.x > 7 || new_coords.x < 0 || new_coords.y > 7 || new_coords.y < 0)
            continue;
        if (filter == TypeFilter::Legal)
        {
            const uint64_t attack_mask = 1ULL << ((new_coords.y * 8) + new_coords.x);
            const bool enpassant = (board.enpassant & attack_mask);

            if (!enpassant && !EnemySquares(new_coords, board, is_white))
                continue;
        }
        moves.emplace_back(new_coords);
    }
    if (filter != Legal)
        return;

    // now advance position
    for (size_t i = 1; i <= advance; i++)
    {
        const BoardCoordinates new_coords = {
            .x = origin.x,
            .y = static_cast<int8_t>(origin.y + static_cast<int8_t>(i) * direction),
        };
        if (new_coords.y > 7 || new_coords.y < 0)
            break;
        if (OcuppiedSquares(new_coords, board))
            break;
        moves.emplace_back(new_coords);
    }
}

void BoardAPI::castlingMoves(BoardCoordinates origin, const BitWiseBoard &board, MaxMovesArray &moves, bool is_white, uint64_t attack_mask, TypeFilter filter)
{
    if (filter != Legal || board.king_check)
        return;

    bool can_castle = is_white ? board.white_can_castle_kingside || board.white_can_castle_queenside : board.black_can_castle_kingside || board.black_can_castle_queenside;
    if (!can_castle)
        return;
    // i should check first the
    // i should check first the queen side or what? :///
    struct CastlingRight
    {
        int dx;
        int distance;
        BoardCoordinates origin;
        BoardCoordinates safe_castling;
        bool castling_right;
    };
    std::array<CastlingRight, 2> castling = {
        CastlingRight{
            .dx = 1,
            .distance = 2,
            .origin = {
                .x = 7,
                .y = is_white ? (int8_t)7 : (int8_t)0,
            },
            .safe_castling = {
                .x = 6,
                .y = is_white ? (int8_t)7 : (int8_t)0,

            },
            .castling_right = is_white ? board.white_can_castle_kingside : board.black_can_castle_kingside,
        },
        CastlingRight{
            .dx = (-1),
            .distance = 3,
            .origin = {
                .x = 0,
                .y = is_white ? (int8_t)7 : (int8_t)0,
            },
            .safe_castling = {
                .x = 2,
                .y = is_white ? (int8_t)7 : (int8_t)0,

            },
            .castling_right = is_white ? board.white_can_castle_queenside : board.black_can_castle_queenside,
        },
    };
    for (CastlingRight &v : castling)
    {
        if (!v.castling_right)
            continue;
        // path is clear
        for (int i = 1; i <= v.distance; i++)
        {
            int8_t new_x = origin.x + v.dx * i;
            uint64_t mask = 1ULL << ((origin.y * 8) + new_x);
            BoardCoordinates new_coords = {
                .x = new_x,
                .y = origin.y,
            };
            if (i <= 2 && (attack_mask & mask)) //  its being attacked?
                break;
            if ((OcuppiedSquares(new_coords, board))) // the square is occupied
                break;
            if (i != v.distance) // we havent arrived to our point :)
                continue;
            moves.emplace_back(v.safe_castling);
        }
    }
}
