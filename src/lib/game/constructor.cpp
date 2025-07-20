#include "game.h++"

ChessGame::ChessGame(std::string fen)
{
    bitwise_board = board.BuildFromFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8   ");
    pieces = board.GetPieces(bitwise_board);

    for (float j = 0; j < 8; j++)
    {
        for (float i = 0; i < 8; i++)
        {
            squares[j * 8 + i] = {i * 50, j * 50, 50, 50};
        }
    }
}