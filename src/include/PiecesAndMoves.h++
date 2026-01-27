#ifndef PIECES_H
#define PIECES_H

enum Pieces {
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

enum GameStates {
  CONTINUE = 0,
  DRAW,
  CHECKMATE,

};

struct PiecesDireciton {
  int x;
  int y;
};
// okay so lets try to improve the research algorithm
#define PAWN_VALUE 100
#define KNIGHT_VALUE 300
#define BISHOP_VALUE 325
#define ROOK_VALUE 500
#define QUEEN_VALUE 900
#endif