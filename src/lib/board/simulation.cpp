#include "board-api.h++"
#include <bitset>
uint64_t BoardAPI::getAttackedSquares(const BitWiseBoard &board, std::optional<bool> is_white)
{
    uint64_t attack_mask = 0;
    bool enemy_to_move = !is_white.value_or(board.white_to_move);
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            if (!FriendSquares(piece_coords, board, enemy_to_move))
            {
                continue;
            }

            MaxMovesArray moves = this->GetMoves(piece_coords, board, enemy_to_move, TypeFilter::Defendable);
            attack_mask |= moves.movement_map;
        }
    }
    return attack_mask;
}

uint64_t BoardAPI::getPotentialAttacks(const BitWiseBoard &board, std::optional<bool> is_white)
{
    uint64_t attack_mask = 0;
    bool white_to_move = is_white.value_or(board.white_to_move);
    bool enemy_to_move = !white_to_move;
    uint64_t our_king = (white_to_move ? board.white_pieces : board.black_pieces) & board.kings;
    for (int8_t y = 0; y < 8; y++)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            if (!FriendSquares(piece_coords, board, enemy_to_move))
            {
                continue;
            }

            MaxMovesArray moves = this->GetMoves(piece_coords, board, enemy_to_move, TypeFilter::Line);
            // we are going to check if first it actually hits on here;

            if (moves.movement_map & our_king)
            {
                attack_mask |= moves.movement_map;
            }
        }
    }
    return attack_mask;
}

// this
bool BoardAPI::IsChecked(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board, bool from_white)
{
    BitWiseBoard new_board = MakeMove(from, to, board, TypeGame::Simulation);
    // first i get the attacked squares
    uint64_t attacked_squares = getAttackedSquares(new_board, from_white);
    // then i make the friendly mask
    uint64_t friendly_mask = from_white ? new_board.white_pieces : new_board.black_pieces;
    uint64_t king_mask = new_board.kings;

    return attacked_squares & friendly_mask & king_mask; // if this gives any value it will return true :) its an intersection basically (a()b()c)
}

bool BoardAPI::movementIsLegal(const BoardCoordinates &from, const BoardCoordinates &to, const BitWiseBoard &board)
{
    MaxMovesArray legal_moves = GetMoves(from, board, board.white_to_move);
    // SO THIS IS JUT TO FILTER ANY KIND OF BULLSHIT SO I DONT HAVE TO LOSE ANY TIME CALCULATING SHIT :)
    for (size_t i = 0; i < legal_moves.size(); i++)
    {
        BoardCoordinates move = legal_moves[i];
        if (move.x == to.x && move.y == to.y)
        {
            return true;
        }
    }
    return false;
}