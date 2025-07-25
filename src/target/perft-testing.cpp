#include "board-api.h++"
#include <vector>
#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include <filesystem>
#include <array>
#include <chrono>
BoardAPI api;
struct PositionalInfo
{
    BitWiseBoard board;
    BoardCoordinates from;
    BoardCoordinates to;
    std::string previous_fen;
};

void ExecuteMoves(const BitWiseBoard &board, BoardCoordinates from, MaxMovesArray &to_moves, std::vector<PositionalInfo> &new_positions)
{
    static Pieces possible_pieces[] = {Pieces::KNIGHT, Pieces::BISHOP, Pieces::ROOK, Pieces::QUEEN};
    for (size_t i = 0; i < to_moves.size(); i++)
    {
        const BoardCoordinates to = to_moves[i];
        BitWiseBoard new_board = api.MakeMove(from, to, board, TypeGame::Bot);

        if (api.IsReadyToPromote(new_board))
        {
            for (size_t j = 0; j < 4; j++)
            {
                Pieces p = possible_pieces[j];

                BitWiseBoard promotion_board = api.Promotion(to, new_board, p);
                new_positions.emplace_back(PositionalInfo{
                    .board = promotion_board,
                    .from = from,
                    .to = to,
                    .previous_fen = api.GetFen(board),
                });
            }
            continue;
        }
        new_positions.emplace_back(PositionalInfo{
            .board = new_board,
            .from = from,
            .to = to,
            .previous_fen = api.GetFen(board),

        });
    }
}
void EvaluatePossiblePosition(const BitWiseBoard &board, std::vector<PositionalInfo> &new_positions)
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

std::vector<PositionalInfo> GetAllPossiblePositions(const std::vector<PositionalInfo> &positions)
{
    std::vector<PositionalInfo> new_positions;

    for (PositionalInfo info : positions)
    {
        EvaluatePossiblePosition(info.board, new_positions);
    }
    return new_positions;
}
std::string BoardCoordinate2String(BoardCoordinates pos)
{
    std::string out = "";
    out += static_cast<char>('a' + pos.x);
    out += static_cast<char>('8' - pos.y);
    return out;
}
void EvaluateFen(size_t depth, std::string fen, const std::vector<size_t> &quantity, const std::string name)
{
    if (!std::filesystem::is_directory("./moves-test"))
    {
        std::filesystem::create_directory("./moves-test");
    }
    if (depth > quantity.size())
    {
        std::cout << "there isnt enough data to test if it actually captures what you want :(\n";
    }
    std::vector<PositionalInfo> positions = {PositionalInfo{.board = api.BuildFromFEN(fen), .from = {0, 0}, .to = {0, 0}, .previous_fen = ""}};
    std::cout << "Testing fen:     " << fen << "\n";
    std::cout << "Expected epochs: " << depth << "\n";
    std::cout << "Name:            " << name << "\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Depth: " << 0 << " Result:" << positions.size() << " positions\n";

    for (size_t epoch = 0; epoch < depth; epoch++)
    {
        auto start = std::chrono::high_resolution_clock::now();

        positions = GetAllPossiblePositions(positions);
        auto end = std::chrono::high_resolution_clock::now();
        bool passed = positions.size() == quantity[epoch];
        std::cout << "Depth: " << epoch + 1
                  << " Result:" << positions.size()
                  << " positions | passed: " << (passed ? "✅" : "⁉️")
                  << " expected:" << quantity[epoch]
                  << " duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";
        if (passed)
            continue;
        std::ofstream file;
        file.open(
            std::format("moves-test/{}-{}.txt", name, epoch + 1));

        for (PositionalInfo &info : positions)
        {

            file << api.GetFen(info.board) << "," << BoardCoordinate2String(info.from) << "," << BoardCoordinate2String(info.to) << "," << info.previous_fen << "\n";
        }
        file.close();
    }
    std::cout << "-----------------------------------\n\n";
}

void GeneralEvaluation()
{
    std::cout << "-----------------------------------\n";
    struct Eval_t
    {
        std::string fen;
        std::vector<size_t> quantity;
        std::string name;
    };
    std::array<Eval_t, 6> fen_eval = {
        Eval_t{
            .fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
            .quantity = {20, 400, 8902, 197281, 4865609},
            .name = "basic-board"},
        Eval_t{
            .fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0",
            .quantity = {48, 2039, 97862, 4085603, 193690690},
            .name = "second", // second is the prblem ...
        },
        Eval_t{
            .fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
            .quantity = {14, 191, 2812, 43238, 674624},
            .name = "third",
        },
        Eval_t{
            .fen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
            .quantity = {6, 264, 9467, 422333, 15833292},
            .name = "fourth",
        },
        Eval_t{
            .fen = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
            .quantity = {44, 1486, 62379, 2103487, 89941194},
            .name = "fifth",
        },
        Eval_t{
            .fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
            .quantity = {46, 2079, 89890, 3894594, 164075551},
            .name = "sixth",
        },
    };

    for (const auto &eval : fen_eval)
    {
        EvaluateFen(4, eval.fen, eval.quantity, eval.name);
    }
}
int main()
{
    GeneralEvaluation();
    /*
    struct Eval_t
    {
        std::string fen;
        std::vector<size_t> quantity;
        std::string name;
    };
    Eval_t eval = {
        .fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        .quantity = {
            20,
            400,
            8902,
            197281,
            4865609,
            119060324,
            3195901860	
        },
        .name = "basic-board"};

    EvaluateFen(5, eval.fen, eval.quantity, eval.name);
    */
    return 0;
}
