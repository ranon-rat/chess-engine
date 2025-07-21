#include <board-api.h++>
#include <bitset>

ArrayPieces InitPossibleMoves()
{
    std::array<std::vector<Move>, Pieces::PIECE_COUNT - 1> moves;
    moves[Pieces::KNIGHT] = {
        {2, 1}, // up right
        {2, -1},
        {-2, 1},
        {-2, -1},
        {1, 2},
        {1, -2},
        {-1, 2},
        {-1, -2}};
    moves[Pieces::BISHOP] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1}};
    moves[Pieces::ROOK] = {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}};
    moves[Pieces::QUEEN] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}};
    moves[Pieces::KING] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}};
    return moves;
}

