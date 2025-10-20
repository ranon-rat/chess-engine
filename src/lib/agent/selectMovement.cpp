#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "engine.h++"
#include <cstddef>
#include <board-api.h++>
#include <cstdlib>
#include <iterator>

Move *Engine::SelectMovement(const BitWiseBoard &board)
{

    Movements moves = api.GetLegalMoves(board);
    if (moves.size() == 0)
        return nullptr;

    size_t pos = rand() % moves.size();
    //auto it= moves.begin();
    //std::advance(it,pos);
    //Move *m=&(*it);
    Move *m= &(moves[pos]);
    return m;
}
