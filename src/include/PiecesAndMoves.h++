#ifndef PIECES_H
#define PIECES_H

enum Pieces
{
    KNIGHT = 0,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    PAWN,
    PIECE_COUNT,
    ENPASSANT,
    NONE,
    UNKOWN

};

enum GameStates
{
    CONTINUE = 0,
    DRAW,
    CHECKMATE,

};

struct Move
{
    int x;
    int y;
};
#endif