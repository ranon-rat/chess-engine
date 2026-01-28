#ifndef BITWISEBOARD_H
#define BITWISEBOARD_H

#include <array>
#include <cstddef>
#include <cstdint>

#define initialize_board 0x0000000000000000
#define pawn_positions 0x00ff00000000ff00
#define PROMOTION_LINES 0xff000000000000ff
#define BLACK_TO_MOVE 0
#define WHITE_TO_MOVE 1
typedef std::array<size_t, 2> count_pieces;

struct BitWiseBoard {
  // es para un chess engine asi que es obvio muchas cosas
  // 8x8 board
  // 64 squares
  // we need to check the basic positions first :)
  // aqui probablemente seria buena idea ma nejarlo por indices?
  // tal vez haya una mejor manera
  count_pieces pawns_c{0, 0};
  count_pieces knights_c{0, 0};
  count_pieces bishops_c{0, 0};
  count_pieces rooks_c{0, 0};
  count_pieces queens_c{0, 0};

  uint64_t pawns = initialize_board;
  uint64_t knights = initialize_board;
  uint64_t bishops = initialize_board;
  uint64_t rooks = initialize_board;
  uint64_t queens = initialize_board;
  uint64_t kings = initialize_board; // obviously 2 kings but we need to store
                                     // the position of both

  // now i should add a new little thing :)
  uint64_t enpassant = initialize_board;
  //  then i need to check on the utilized squares
  uint64_t white_pieces = initialize_board;
  uint64_t black_pieces = initialize_board;
  // attacked squares
  uint64_t attacked_squares =
      initialize_board; // this is for the attacked squares of the opponent
                        // pieces :)
  uint64_t potenital_attacks = initialize_board;
  uint64_t defended_squares = initialize_board;
  // some basic rights
  bool white_to_move : 1 = true;
  // castling bs :)

  bool white_can_castle_kingside : 1 = false;
  bool white_can_castle_queenside : 1 = false;
  bool black_can_castle_kingside : 1 = false;
  bool black_can_castle_queenside : 1 = false;

  // no capture, or movement of pawn

  uint8_t half_move = 0;
  uint16_t complete_move = 0;
  // okay so here is some things that we need

  bool king_check : 1 = false; // so this will become important later on
  uint64_t zobrist = 0;
};
#endif