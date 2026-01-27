#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include "board-api.h++"
#include <cstdint>
#include <optional>
#define BLACK_SIDE 0xff00000000000000
#define WHITE_SIDE 0x00000000000000ff
#define PRE_PROMOTION_LINE 0x00ff00000000ff00
#define PROMOTION_LINE 0xff000000000000ff
// this is for the bot
bool BoardAPI::CanPromote(const BoardCoordinates &from,
                          const BoardCoordinates &to,
                          const BitWiseBoard &board) {
  const uint64_t from_mask = (1ULL) << (from.y * 8 + from.x);
  const uint64_t to_mask = (1ULL) << (to.y * 8 + to.x);
  return (PRE_PROMOTION_LINE & from_mask & board.pawns) &&
         (to_mask & PROMOTION_LINE);
}

std::optional<BoardCoordinates>
BoardAPI::whereIsPromotion(const BitWiseBoard &board) {
  // now i need to check something
  if (!IsReadyToPromote(board)) {
    return std::nullopt;
  }
  uint64_t friend_mask =
      board.white_to_move ? board.white_pieces : board.black_pieces;
  int8_t search_y = board.white_to_move ? 0 : 7;
  for (int8_t search_x{0}; search_x < 7; search_x++) {

    uint64_t mask = 1ULL << (search_y * 8 + search_x);
    if (board.pawns & mask & friend_mask) {
      return BoardCoordinates{.x = search_x, .y = search_y};
    }
  }
  return std::nullopt;
}

BitWiseBoard BoardAPI::Promotion(BoardCoordinates from,
                                 const BitWiseBoard &board, Pieces new_piece) {

  const TypePiece piece = GetPieceFromCoord(from, board);
  if (piece.piece != Pieces::PAWN) {
    return board;
  }
  const bool white_to_move = board.white_to_move;
  const uint64_t position_mask = (1ULL) << ((from.y * 8) + from.x);
  const uint64_t finish_mask = white_to_move ? WHITE_SIDE : BLACK_SIDE;

  if (!(finish_mask & position_mask)) {
    return board;
  }

  BitWiseBoard new_board = board;

  new_board.pawns &= ~position_mask;
  new_board.pawns_c[white_to_move]--;

  switch (new_piece) {
  case QUEEN:
    new_board.queens |= position_mask;
    new_board.queens_c[white_to_move]++;
    break;
  case ROOK:
    new_board.rooks |= position_mask;
    new_board.rooks_c[white_to_move]++;

    break;
  case BISHOP:
    new_board.bishops |= position_mask;
    new_board.bishops_c[white_to_move]++;

    break;
  case KNIGHT:
    new_board.knights |= position_mask;
    new_board.knights_c[white_to_move]++;
    break;
  default:
    break;
  }
  new_board.white_to_move = !white_to_move;

  new_board.attacked_squares = getAttackedSquares(new_board);
  new_board.zobrist = GetZobrist(new_board);
  new_board.potenital_attacks = getPotentialAttacks(new_board);
  uint64_t new_friend_mask =
      new_board.white_to_move ? board.white_pieces : board.black_pieces;

  // here we are going to calculate the squares that we could attack :)
  // okay here goes some basic shit :)

  new_board.king_check =
      board.kings & new_friend_mask & new_board.attacked_squares;
  //
  return new_board;
}
