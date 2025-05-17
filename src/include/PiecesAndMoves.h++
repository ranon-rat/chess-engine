#ifndef PIECES_H
#define PIECES_H

enum Pieces
{
    KNIGHT=0,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    PIECE_COUNT,
    PAWN,
    NONE

};

struct Move
{
    int x;
    int y;
};
#endif