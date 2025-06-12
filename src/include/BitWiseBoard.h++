#ifndef BITWISEBOARD_H
#define BITWISEBOARD_H

#include <cstdint>
#define initialize_board 0x0000000000000000
#define pawn_positions 0x00ff00000000ff00
struct BitWiseBoard
{
    // es para un chess engine asi que es obvio muchas cosas
    // 8x8 board
    // 64 squares
    // we need to check the basic positions first :)
    uint64_t pawns = initialize_board;
    uint64_t knights = initialize_board;
    uint64_t bishops = initialize_board;
    uint64_t rooks = initialize_board;
    uint64_t queens = initialize_board;
    uint64_t kings = initialize_board; // obviously 2 kings but we need to store the position of both

    // now i should add a new little thing :)
    uint64_t enpassant = initialize_board;
    //  then i need to check on the utilized squares
    uint64_t utilized_squares = initialize_board; // this is the board with all the pieces
    uint64_t white_pieces = initialize_board;
    uint64_t black_pieces = initialize_board;
    // attacked squares
    uint64_t attacked_squares = initialize_board; // this is for the attacked squares of the opponent pieces :)
    uint64_t potential_line_attacks = initialize_board;
    // some basic rights
    bool white_to_move:1 = true;
    // castling bs :)
    bool black_can_castle:1=true;
    bool white_can_castle:1=true;
    bool white_can_castle_kingside:1 = true;
    bool white_can_castle_queenside:1 = true;
    bool black_can_castle_kingside:1 = true;
    bool black_can_castle_queenside:1 = true;
};
#endif