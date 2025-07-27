#include "board-api.h++"

void BoardAPI::getLegalFromPawn(const BitWiseBoard &board, Movements &output, const BoardCoordinates &from, MaxMovesArray &to_moves)
{
    static Pieces possible_pieces[] = {Pieces::KNIGHT, Pieces::BISHOP, Pieces::ROOK, Pieces::QUEEN};

    for (size_t i = 0; i < to_moves.size(); i++)
    {
        BoardCoordinates to = to_moves[i];
        if (!CanPromote(from, to, board))
        {
            output.emplace_back(Move{
                .from = from,
                .to = to,
                .promotion = NONE,
            });
            continue;
        }
        for (Pieces &p : possible_pieces)
        {
            output.emplace_back(Move{
                .from = from,
                .to = to,
                .promotion = p,
            });
        }
    }
}
void BoardAPI::getLegalFromRest( Movements &output, const BoardCoordinates &from, MaxMovesArray &to_moves)
{
    for (size_t i = 0; i < to_moves.size(); i++)
    {
        BoardCoordinates to = to_moves[i];
        output.emplace_back(Move{
            .from = from,
            .to = to,
            .promotion = NONE,
        });
    }
}

Movements BoardAPI::GetLegalMoves(const BitWiseBoard &board)
{
    Movements output;
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            BoardCoordinates from = {.x = x, .y = y};
            MaxMovesArray to_moves = GetMoves(from, board,TypeFilter::Legal);
            if (to_moves.size() == 0)
                continue;

            /// i should verify if from is mask

            uint64_t from_mask(1ull << (y * 8 + x));
            if (from_mask & board.pawns)
            {
                getLegalFromPawn(board, output, from, to_moves);
                continue;
            }
            getLegalFromRest( output, from, to_moves);
        }
    }
    return output;
}
