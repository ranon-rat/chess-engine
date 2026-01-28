#include "BitWiseBoard.h++"
#include "PiecesAndMoves.h++"
#include "engine.h++"


int Engine::evaluate(const BitWiseBoard &board) {
  // okay so lets start to observe
  // this may be interesting due to the way it works
  // this is just evaluating a position that could be used so
  // i can use it for later purposes
  // i can penalize the position if it has a check in it, or
  // i can also do other stuff but this is just pure positional evaluation
  int white_eval = count_material(board, true);
  int black_eval = count_material(board, false);
  int evaluation = white_eval - black_eval;
  int perspective = board.white_to_move ? 1 : -1;

  return evaluation * perspective;
}
// i need an ordering board for positions, i will be working on that.

int Engine::count_material(const BitWiseBoard &board, bool white_to_move) {
  int material{0};
  material += board.pawns_c[white_to_move] * PAWN_VALUE;
  material += board.knights_c[white_to_move] * KNIGHT_VALUE;
  material += board.bishops_c[white_to_move] * BISHOP_VALUE;
  material += board.rooks_c[white_to_move] * ROOK_VALUE;
  material += board.queens_c[white_to_move] * QUEEN_VALUE;

  return material;
}
