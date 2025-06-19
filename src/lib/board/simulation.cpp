#include "board.h++"
#include <iostream>
uint64_t Board::GetAttackPotentialSquares(const BitWiseBoard &board, TypeFilter filter,bool on_enemy)
{
    uint64_t attack_mask = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            // if the square is the one of a friend then i can continue :)
            if ((!FriendSquares(piece_coords, board)&&!on_enemy)||(!EnemySquares(piece_coords,board)&&on_enemy))
            {
                continue;
            }
            std::vector<BoardCoordinates> moves = this->GetMoves(piece_coords, board, filter);
            for (BoardCoordinates &move_coords : moves)
            {
                attack_mask |= (1ULL) << ((move_coords.y * 8) + move_coords.x);
            }
        }
    }
    return attack_mask;
}

uint64_t Board::GetAttackedSquares(const BitWiseBoard &board,bool on_enemy)
{
    return GetAttackPotentialSquares(board, TypeFilter::Defendable,on_enemy);
} 


// this 
bool Board::IsChecked(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board){
    BitWiseBoard new_board=MakeMove(from,to,board,true);
    uint64_t attacked_squares=GetAttackedSquares(new_board);
    uint64_t friendly_mask=board.white_to_move?board.white_pieces:board.black_pieces;
    std::cout<<((attacked_squares&friendly_mask&board.kings)?"its checked":"it isnt checked")<<"\n";
    return attacked_squares&friendly_mask&board.kings; // if this gives any value it will return true :) its an intersection basically (a()b()c)
}
