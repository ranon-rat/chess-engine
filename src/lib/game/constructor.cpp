#include "game.h++"

ChessGame::ChessGame(std::string fen)
{
    bitwise_board = board.BuildFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0");
    pieces = board.GetPieces(bitwise_board);

    for (float j = 0; j < 8; j++)
    {
        for (float i = 0; i < 8; i++)
        {
            squares[j * 8 + i] = {i * 50, j * 50, 50, 50};
        }
    }
}