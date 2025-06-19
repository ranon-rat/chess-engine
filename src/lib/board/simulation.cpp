#include <board.h++>
uint64_t Board::GetAttackPotentialSquares(const BitWiseBoard &board, TypeFilter filter)
{
    uint64_t attack_mask = 0ULL;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            // if the square is the one of a friend then i can continue :)
            if (!FriendSquares(piece_coords, board))
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

uint64_t Board::GetAttackedSquares(const BitWiseBoard &board)
{
    return GetAttackPotentialSquares(board, TypeFilter::Defendable);
} // so with this we get somethign quite basic on how this shit will
uint64_t Board::GetPotentialAttackSquares(const BitWiseBoard &board)
{
    return GetAttackPotentialSquares(board, TypeFilter::Possible);
}


// this 
uint64_t Board::SimulatePosition(BoardCoordinates from, BoardCoordinates to, const BitWiseBoard &board){

}
