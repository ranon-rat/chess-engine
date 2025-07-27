#include "game.h++"
#include <iostream>
ChessGame::ChessGame(std::string fen)
{
    // test
    std::cout<<fen<<"\n";
    bitwise_board = api.BuildFromFEN("r2k3r/p1ppqpb1/bn2Pnp1/4N3/1p2P3/2NQ3p/PPPBBPPP/R3K2R b KQ - 0 1");
    we_are_white=bitwise_board.white_to_move;
    //bitwise_board = board.BuildFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    pieces = api.GetPieces(bitwise_board);

    for (float j = 0; j < 8; j++)
    {
        for (float i = 0; i < 8; i++)
        {
            squares[j * 8 + i] = {i * 50, j * 50, 50, 50};
        }
    }
}