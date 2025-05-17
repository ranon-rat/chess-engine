#ifndef BITWISEBOARD_H
#define BITWISEBOARD_H

#include <cstdint>
#define initialize_board 0x0000000000000000

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
    //  then i need to check on the utilized squares
    uint64_t utilized_squares = initialize_board; // this is the board with all the pieces
    uint64_t white_pieces = initialize_board;
    uint64_t black_pieces = initialize_board;
    // attacked squares
    uint64_t attacked_squares = initialize_board; // this is for the attacked squares of the opponent pieces :)
    // some basic rights
    bool white_to_move = true;
    bool white_castle = true;
    bool black_castle = true;
    
};
#endif