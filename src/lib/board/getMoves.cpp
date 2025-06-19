#include "board.h++"
#include <iostream>
std::vector<BoardCoordinates> Board::GetMoves(BoardCoordinates piece, const BitWiseBoard &board,std::optional<bool> is_white, TypeFilter filter)
{
    // checking to//
    // now we need to check if the piece is a pawn or not
    
    bool from_white=(is_white.value_or(board.white_to_move));
    //uint64_t attack_mask=GetAttackedSquares(board,from_white);
   
    std::vector<BoardCoordinates> moves;
    // hmmm
    // first lets check if the piece is one of us
    if (!(FriendSquares(piece, board,from_white)))
    {
        return moves;
    }

    // avaible pieces
    // uint64_t avaible_pieces=0;
    TypePiece piece_info = GetPieceFromCoord(piece, board);
    switch (piece_info.piece)
    {
    case Pieces::PAWN:
        PawnMoves(piece, board, moves,from_white, filter);
        break;
    case Pieces::KNIGHT:
        OneLineMoves(KNIGHT, piece, board, moves,from_white, filter);
        break;
    case Pieces::BISHOP:
        LineMoves(BISHOP, piece, board, moves,from_white, filter);
        break;
    case Pieces::ROOK:
        LineMoves(ROOK, piece, board, moves,from_white, filter);
        break;
    case Pieces::QUEEN:
        LineMoves(QUEEN, piece, board, moves,from_white, filter);
        break;
    case Pieces::KING:
        OneLineMoves(KING, piece, board, moves,from_white, filter);
        CastlingMoves(piece, board, moves,from_white,board.attacked_squares, filter);
        break;
    default:
        break;
    }

    if (filter != Legal)
    {
        return moves;
    }
    std::vector<BoardCoordinates> filtered_moves = {};
    for (BoardCoordinates &move : moves)
    {
        if (IsChecked(piece, move, board,from_white))
        {
            continue;
        }
        filtered_moves.emplace_back(move);
    }
    return filtered_moves;
}

void Board::LineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves,bool is_white, TypeFilter filter)
{
    // this is only for the queen
    // the rook and the bishop
    for (auto move : m_possible_moves[piece])
    { // i first create the vector

        for (int x = origin.x + move.x,
                 y = origin.y + move.y;
             x < 8 && x >= 0 &&
             y < 8 && y >= 0;
             x += move.x, y += move.y)
        {

            BoardCoordinates coords = {
                .x = x,
                .y = y,
            };
            if (FriendSquares(coords, board,is_white))
            {
                if (filter == Defendable)
                {
                    moves.emplace_back(coords);
                }
                break;
            }

            moves.emplace_back(coords);
            if (EnemySquares(coords, board,is_white))
            {

                break;
            }
        }
    }
}

void Board::OneLineMoves(Pieces piece, BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves,bool is_white, TypeFilter filter)
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
        if (FriendSquares(coords, board,is_white) && (filter == Legal))
        {
            continue;
        }

        moves.emplace_back(coords);
    }
}

void Board::PawnMoves(BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves,bool is_white, TypeFilter filter)
{

    int direction = board.white_to_move ? -1 : 1; // this is important :)
    uint64_t pawn_mask = 1ULL << ((origin.y * 8) + origin.x);
    bool long_move_right = (pawn_mask & pawn_positions) > 0;
    int advance = long_move_right ? 2 : 1;

    // attack position :)
    for (int i = -1; i <= 1; i += 2)
    {
        const BoardCoordinates new_coords = {
            .x = origin.x + i,
            .y = origin.y + direction};

         if(new_coords.x>=8||new_coords.x<0||new_coords.y>=8||new_coords.y<=0){
            continue;
         }

        const uint64_t attack_mask = board.enpassant & 1ULL << ((new_coords.y * 8) + new_coords.x);
        const bool enpassant = (board.enpassant & attack_mask) > 0;

        if (!EnemySquares(new_coords, board,is_white) && !enpassant && filter == TypeFilter::Legal)
        {
            continue;
        }
        moves.emplace_back(new_coords);
    }
    if (filter != Legal)
    {
        return;
    }
    // now advance position
    for (int i = 1; i <= advance; i++)
    {
        const BoardCoordinates new_coords = {
            .x = origin.x,
            .y = origin.y + i * direction,
        };
        if (new_coords.y >= 8||new_coords.y<0)
            break;
        if (OcuppiedSquares(new_coords, board))
        {
            break;
        }
        moves.emplace_back(new_coords);
    }
}

void Board::CastlingMoves(BoardCoordinates origin, const BitWiseBoard &board, std::vector<BoardCoordinates> &moves,bool is_white,uint64_t attack_mask, TypeFilter filter)
{
    if (filter != Legal)
    {
        std::cout<<"SOWWY\n";   
        return;
    }

    bool can_castle = board.white_to_move ? board.white_can_castle_kingside || board.white_can_castle_queenside : board.black_can_castle_kingside || board.black_can_castle_queenside;
    if (!can_castle)
    {
        std::cout << "cant castle\n";
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

            std::cout << "NOT THIS ONE " << v.dx << "\n";
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
            
            if ((attack_mask & mask)      || (OcuppiedSquares(new_coords, board)&& i!=0 ))
            {
                std::cout<<"we are covered!\n";
                std::cout<<((attack_mask & mask)?"WE ARE ATTACKED":"WE ARENT ATTACKED")<<"\n";
                std::cout<<"NOT AVAIBLE\n";
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
