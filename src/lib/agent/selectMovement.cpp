#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include "engine.h++"
#include <algorithm>
#include <board-api.h++>

#include <optional>

std::optional<Move> Engine::SelectMovement(const BitWiseBoard &board) {
  Movements moves = api.GetLegalMoves(board);
  if (moves.size() == 0)
    return std::nullopt;

  auto alpha = int{-1'000'000'000};
  auto beta = int{1'000'000'000};
  auto depth = int{5};
  auto best_index{0uz};
  for (auto i{0uz}; i < moves.size(); i++) {
    BitWiseBoard new_board = api.EvalBoard(moves[i], board);
    int eval = -search(new_board, depth, -beta, -alpha); // ✅ Negamax

    if (eval > alpha) {
      best_index = i;
      alpha = eval;
    }
  }
  return moves[best_index];
}

int Engine::search(const BitWiseBoard &board, int depth, int alpha, int beta) {
  GameStates game_state = api.CheckBoardState(board);

  if (game_state == CHECKMATE) {
    return -100000 + depth; // Prefer faster mates
  }
  if (game_state == DRAW) {
    return 0;
  }

  if (depth <= 0) {
    return evaluate(board);
  }

  Movements moves = api.GetLegalMoves(board);
  std::sort(moves.begin(), moves.end(),
            [this, board](const Move &m1, const Move &m2) {
              return evaluate_move(board, m1) > evaluate_move(board, m2);
              //
            });
  for (auto move : moves) {
    int result = -search(api.EvalBoard(move, board), depth - 1, -beta, -alpha);

    if (result >= beta) {
      return beta; // Beta cutoff
    }
    alpha = std::max(result, alpha);
  }

  return alpha;
}
