#include <board.h++>

GameStates Board::CheckBoardState(const BitWiseBoard &board)
{ // draw by repetition is something that i have to add :)

    // i think that now i will do the next
    // get the possible moves that i will have for each square(well i have to check if the piece that i am using actually has what i want :)
    // then i have to check if the king is in check
    // and then i will have to check if its not in check but he cannot move
    // if he cannot move is a draw and he is not getting attacked is a draw
    // if he cannot move and he is getting attacked then is a checkmate, if it isnt then we can continue :)
// i should take into consideration some points or at least some positions where we cant really do something or solve any of this shit but for now i think that this seems good enough 
    bool is_getting_attacked = board.attacked_squares & board.kings & GetUtilizedSquares(board);
    bool friend_mask = board.white_to_move ? board.white_pieces : board.black_pieces;
    BoardCoordinates king_coordinates = { 
        .x = -1,
        .y = -1,
    };
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            uint64_t mask = (1ULL) << ((y * 8) + x);
            if (!(mask & friend_mask & board.kings))
            {
                continue;
            }
            king_coordinates.x = x;
            king_coordinates.y = y;
            break;
        }
    }
    bool more_pieces_can_move=false;
    for(int8_t y=0;y<8 && !more_pieces_can_move;y++){
        for(int8_t x=0;x<8&&!more_pieces_can_move;x++){
            more_pieces_can_move=GetMoves(BoardCoordinates{.x=x,.y=y},board).size()>0;
        }
    }
    if (king_coordinates.x == -1)// this just means we didnt found anything
    {
        return GameStates::CHECKMATE;
    }
    if (more_pieces_can_move)// this just means that we are on check but we can still do something :)
    {
        return GameStates::CONTINUE;    
    };

    if(is_getting_attacked){// we are fucked dude
        return GameStates::CHECKMATE;
    }
    return GameStates::DRAW;// at least it was a draw less goooo
}
