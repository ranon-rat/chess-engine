#include "board-api.h++"
#include <vector>
#include <iostream>
#include <format>
BoardAPI api;

int main()
{

    // the fen works!

    std::vector<std::string> fens = {

        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - a1 0 1",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq a1 0 1",
        "rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3",

    };
    for (std::string fen : fens)
    {
        BitWiseBoard board = api.BuildFromFEN(fen);
        std::string new_fen = api.GetFen(board);
        std::cout << "-------------------------------\n";
        std::cout << "testing the next fen: " << fen << "\n";
        std::cout << "returned fen:         " << new_fen << "\n";
        std::cout << "passed?               " << (fen == new_fen ? "✅" : "⁉️") << "\n";
        std::cout << "-------------------------------\n";
    }

    return 0;
}