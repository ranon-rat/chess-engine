#include "board.h++"
#include <bitset>
uint64_t Board::getAttackedSquares(const BitWiseBoard &board, std::optional<bool> is_white)
{
    uint64_t attack_mask = 0;
    bool white_to_move = !is_white.value_or(board.white_to_move);
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            if (!FriendSquares(piece_coords, board, white_to_move))
            {
                continue;
            }

            MaxMovesArray moves = this->GetMoves(piece_coords, board, white_to_move, TypeFilter::Defendable);
            for (size_t i = 0; i < moves.size(); i++)
            {
                BoardCoordinates move_coords = moves[i];
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
    uint64_t attacked_squares = getAttackedSquares(new_board, from_white);
    // then i make the friendly mask
    uint64_t friendly_mask = from_white ? new_board.white_pieces : new_board.black_pieces;
    uint64_t king_mask = new_board.kings;

    return attacked_squares & friendly_mask & king_mask; // if this gives any value it will return true :) its an intersection basically (a()b()c)
}
