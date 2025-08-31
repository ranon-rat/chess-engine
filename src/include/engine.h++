#ifndef CHESS_ENGINE_HPP
#define CHESS_ENGINE_HPP
#include "board-api.h++"
class Engine
{
public:
    BoardAPI api;
    /*
    vamos a pensar un poco que es lo que vamos a hacer
    primero, lo que tengo que hacer es tener una tabla 
    donde tener los valores que voy a estar usando
    tengo que tener una forma de poder evaluar la posicion y poder llamarla al ejecutar el programa
    pero como lo voy a hacer?
 
    

    Bueno la idea es que esto lo saque de un archivo
    vamos a llamarlo idk


    [name-piece]-[epoch].txt

    como podemos sacar el epoch?
    puedo sacar la cantidad de piezas que tienen
    y tambien puedo tomar en cuenta la cantidad de piezas
    y puedo multiplicarlo por 
    */
    
    Move *SelectMovement(const BitWiseBoard &board);
private:
    float evaluate_pieces_positions(const BitWiseBoard &board) {}

};
#endif