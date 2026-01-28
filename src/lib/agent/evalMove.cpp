#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include "engine.h++"

int Engine::evaluate_move(const BitWiseBoard &board, const Move &move) {
  int moveScoreGuess{0};

  auto from_piece = api.GetPieceFromCoord(move.from, board);
  auto to_piece = api.GetPieceFromCoord(move.to, board);
  if (to_piece.piece != Pieces::NONE) {
    moveScoreGuess = 10 * getPieceValue(to_piece.piece, move.to) -
                     getPieceValue(from_piece.piece, move.from);
  }
  if (move.promotion != Pieces::NONE) {
    moveScoreGuess += getPieceValue(move.promotion, move.to);
  }
  // okay i have to  be working on some stuff
  // first it would be nice to have a work around to defend our pieces
  // stuff

  /*
  # TODO

  i need to have a way to know in which squares i should be focused for the
  defense line. Knowing which squares can be defended would be vital specially
  if i am working with this kind of data structures it would be nice to have
  connected squares.

  I also need to have a map to know in which squares i should be the most
  worried about, since there is a slight problem with the squares that can be
  attacked by the pawns


  I also need to improve other stuff
  */
  return moveScoreGuess;
}

int Engine::getPieceValue(Pieces piece, const BoardCoordinates) {
  if (piece > 0 && piece < piece_value.size()) {
    return piece_value[piece];
  }

  return 0;
}