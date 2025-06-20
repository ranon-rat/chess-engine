#include "board.h++"
#include <iostream>
#include <bitset>
uint64_t Board::GetAttackedSquares(const BitWiseBoard &board, std::optional<bool> is_white)
{
    uint64_t attack_mask = 0;
    bool white_to_move = is_white.value_or(board.white_to_move);
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            if (!FriendSquares(piece_coords, board, white_to_move))
            {
                continue;
            }

            std::vector<BoardCoordinates> moves = this->GetMoves(piece_coords, board, white_to_move, TypeFilter::Defendable);
            for (BoardCoordinates &move_coords : moves)
            {
                uint64_t mask = (1ULL) << ((move_coords.y * 8) + move_coords.x);
                attack_mask |= mask;
            }
        }
    }
    return attack_mask;
}

// this
bool Board::IsChecked(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board, bool from_white)
{
    BitWiseBoard new_board = MakeMove(from, to, board, true);
    // first i get the attacked squares
    uint64_t attacked_squares = GetAttackedSquares(new_board, !from_white);
    // then i make the friendly mask
    uint64_t friendly_mask = from_white ? new_board.white_pieces : new_board.black_pieces;
    uint64_t king_mask=new_board.kings;
  
    std::cout << ((attacked_squares & friendly_mask & new_board.kings) ? "its checked" : "it isnt checked") << "\n";
    return attacked_squares & friendly_mask & king_mask; // if this gives any value it will return true :) its an intersection basically (a()b()c)
}
