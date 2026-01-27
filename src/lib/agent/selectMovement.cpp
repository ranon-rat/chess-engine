#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include "engine.h++"
#include <algorithm>
#include <board-api.h++>
#include <cstddef>
#include <cstdlib>
#include <optional>

std::optional<Move> Engine::SelectMovement(const BitWiseBoard &board) {
  Movements moves = api.GetLegalMoves(board);
  if (moves.size() == 0)
    return std::nullopt;
  int perspective = board.white_to_move ? 1 : -1;
  int alpha = -1000000000;
  int beta = 1000000000;
  int depth = 4;
  auto best_index{0uz};
  auto best_eval{0};
  for (auto i{0uz}; i < moves.size(); i++) {
    int eval = search(api.EvalBoard(moves[i], board), depth, alpha, beta)*perspective;
    if (eval > best_eval) {
      best_eval = eval;
      best_index = i;
    }
  }
  return moves[best_index];
}

int Engine::search(const BitWiseBoard &board, size_t depth, int alpha,
                   int beta) {

  if (depth <= 0) {
    return evaluate(board);
  }
  Movements moves = api.GetLegalMoves(board);
  GameStates game_state = api.CheckBoardState(board);
  switch (game_state) {
  case DRAW:
    return 0;
  case CHECKMATE:
    return -100000000;
  case CONTINUE:
    // here i must have the thing that i made for the evaluation

    for (auto move : moves) {
      // the eval board should be able to promote
      int result =
          -search(api.EvalBoard(move, board), depth - 1, -beta, -alpha);
      if (result >= beta) {
        return beta;
      }
      alpha = std::max(result, alpha);
    }
  }

  return alpha;
}

int Engine::evaluate(const BitWiseBoard &board) {
  int white_eval = count_material(board, true);
  int black_eval = count_material(board, false);
  int evaluation = white_eval - black_eval;
  int perspective = board.white_to_move ? 1 : -1;
  return evaluation * perspective;
}
int Engine::count_material(const BitWiseBoard &board, bool white_to_move) {
  int material{0};
  material += board.pawns_c[white_to_move] * PAWN_VALUE;
  material += board.knights_c[white_to_move] * KNIGHT_VALUE;
  material += board.bishops_c[white_to_move] * BISHOP_VALUE;
  material += board.rooks_c[white_to_move] * ROOK_VALUE;
  material += board.queens_c[white_to_move] * QUEEN_VALUE;
  return material;
}
