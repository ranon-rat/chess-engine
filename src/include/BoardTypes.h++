#ifndef BOARDTYPES_HPP
#define BOARDTYPES_HPP
#include "PiecesAndMoves.h++"
#include <cstdint>
#include <array>

struct TypePiece
{
    Pieces piece;
    bool isWhite; // true for white, false for black
};
struct BoardCoordinates
{
    int8_t x = 0;
    int8_t y = 0;
};
// this class is just for storing the ovements and not having to store anything in the heap
class MaxMovesArray
{
private:
    size_t m_size = 0;

    BoardCoordinates m_coords[28];

public:
    uint64_t movement_map = 0;

public:
    MaxMovesArray()
    {
    }
    MaxMovesArray(const MaxMovesArray &other)
    {
        m_size = other.m_size;
        movement_map = other.movement_map;
        for (size_t i = 0; i < other.m_size; i++)
        {
            m_coords[i] = other.m_coords[i];
        }
    }

    MaxMovesArray &operator=(const MaxMovesArray &other)
    {
        if (this == &other)
        {
            return *this;
        }
        movement_map=other.movement_map;
        this->m_size = other.m_size;
        for (size_t i = 0; i < other.m_size; i++)
        {
            m_coords[i] = other.m_coords[i];
        }
        return *this;
    }
    void emplace_back(const BoardCoordinates &coords)
    {
        m_coords[m_size] = coords;
        movement_map|=1ULL<<(coords.y*8+coords.x);
        m_size++;
    }
    size_t size()
    {
        return m_size;
    }

    // Versión para lectura y escritura
    BoardCoordinates &operator[](size_t index)
    {
        return m_coords[index];
    }

    // Versión solo lectura (const)
    const BoardCoordinates &operator[](size_t index) const
    {
        return m_coords[index];
    }
};
#endif