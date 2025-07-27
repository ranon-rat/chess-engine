#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP
#include "board-api.h++"

class Engine
{
public:
BoardAPI api;
    Move* SelectMovement(const BitWiseBoard &board);
};
#endif