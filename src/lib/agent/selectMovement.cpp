#include "engine.h++"

Move *Engine::SelectMovement(const BitWiseBoard &board)
{

    Movements moves = api.GetLegalMoves(board);
    if (moves.size() == 0)
        return nullptr;

    size_t pos = rand() % moves.size();
    return &moves[pos];
}
