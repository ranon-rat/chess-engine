#include "board-api.h++"
#include <vector>
#include <iostream>
#include <string>
#include <format>
BoardAPI api;
void ExecuteMoves(const BitWiseBoard &board, BoardCoordinates from, MaxMovesArray &to_moves, std::vector<BitWiseBoard> &new_positions)
{
    static Pieces possible_pieces[] = {Pieces::KNIGHT, Pieces::BISHOP, Pieces::ROOK, Pieces::QUEEN};
    for (size_t i = 0; i < to_moves.size(); i++)
    {
        const BoardCoordinates to = to_moves[i];
        BitWiseBoard new_board = api.MakeMove(from, to, board);

        if (api.IsReadyToPromote(board))
        {
            for (Pieces p : possible_pieces)
            {
                new_positions.emplace_back(
                    api.Promotion(to, new_board, p));
            }
            continue;
        }
        new_positions.emplace_back(new_board);
    }
}
void EvaluatePossiblePosition(const BitWiseBoard &board, std::vector<BitWiseBoard> &new_positions)
{
    if (api.CheckBoardState(board) != GameStates::CONTINUE)
    {
        return;
    }
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            BoardCoordinates from = {
                .x = x,
                .y = y,
            };
            MaxMovesArray moves = api.GetMoves(from,
                                               board);
            ExecuteMoves(board, from, moves, new_positions);
        }
    }
}

std::vector<BitWiseBoard> GetAllPossiblePositions(const std::vector<BitWiseBoard> &positions)
{
    std::vector<BitWiseBoard> new_positions;

    for (BitWiseBoard board : positions)
    {
        EvaluatePossiblePosition(board, new_positions);
    }
    return new_positions;
}
void EvaluateFen(size_t depth, std::string fen, const std::vector<size_t> &quantity)
{
    if (depth > quantity.size())
    {
        std::cout << "there isnt enough data to test if it actually captures what you want :(\n";
    }
    std::vector<BitWiseBoard> positions = {api.BuildFromFEN(fen)};
    std::cout << "Testing fen:     " << fen << "\n";
    std::cout << "Expected epochs: " << depth << "\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Depth: " << 0 << " Result:" << positions.size() << " positions\n";

    for (size_t epoch = 0; epoch < depth; epoch++)
    {

        positions = GetAllPossiblePositions(positions);
        std::cout << "Depth: " << epoch + 1 << " Result:" << positions.size() << " positions | passed: " << (positions.size() == quantity[epoch] ? "✅" : "⁉️") << "\n";
    }
    std::cout << "-----------------------------------\n\n";
}
int main()
{
    std::cout << "-----------------------------------\n";

    EvaluateFen(2,
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                {20, 400, 8902, 197281, 4865609});

    EvaluateFen(2,
                "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",
                {48, 2039, 97862, 4085603, 193690690});
}
