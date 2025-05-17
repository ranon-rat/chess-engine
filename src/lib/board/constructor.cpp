#include <board.h++>
#include <iostream>
#include <bitset>
Board::Board()
{
    // aqui deberia de ser suficiente :)
    m_possible_moves[Pieces::KNIGHT] = {
        {2, 1}, // up right
        {2, -1},
        {-2, 1},
        {-2, -1},
        {1, 2},
        {1, -2},
        {-1, 2},
        {-1, -2}};
    m_possible_moves[Pieces::BISHOP] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1}};
    m_possible_moves[Pieces::ROOK] = {
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}};
    m_possible_moves[Pieces::QUEEN] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}};
    m_possible_moves[Pieces::KING] = {
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1},
        {1, 0},
        {0, 1},
        {-1, 0},
        {0, -1}};
}

BitWiseBoard Board::BuildFromFEN(std::string fen)
{
    BitWiseBoard board;
    int x = 0;
    int y = 0;
    for (size_t i = 0; i < fen.size(); i++)
    {
        char character = fen[i];
        if (character == ' ')
        {
            break;
        }

        uint8_t value = (character - '0');

        if (value > 0 && value < 9)
        {
            std::cout << value << " ";
            // this means that i need to skip that many squares
            x += value;
            continue;
        }
        if (character == '/')
        {
            // this means that i need to go to the next line
            x = 0;
            y++;
            std::cout << "\n";
            continue;
        }
        uint64_t mask = 1ULL << ((y * 8) + x);
        std::cout << character << " ";
        switch (character)
        {
        case 'p':
            board.pawns |= mask;
            board.black_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        case 'P':
            board.pawns |= mask;
            board.white_pieces |= mask;
            board.utilized_squares |= mask;

            break;
        case 'n':
            board.knights |= mask;
            board.black_pieces |= mask;
            board.utilized_squares |= mask;

            break;
        case 'N':
            board.knights |= mask;
            board.white_pieces |= mask;
            board.utilized_squares |= mask;

            break;
        case 'b':
            board.bishops |= mask;
            board.black_pieces |= mask;
            board.utilized_squares |= mask;

            break;
        case 'B':
            board.bishops |= mask;
            board.white_pieces |= mask;
            board.utilized_squares |= mask;

            break;
        case 'r':
            board.rooks |= mask;
            board.black_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        case 'R':
            board.rooks |= mask;
            board.white_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        case 'q':
            board.queens |= mask;
            board.black_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        case 'Q':
            board.queens |= mask;
            board.white_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        case 'k':
            board.kings |= mask;
            board.black_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        case 'K':
            board.kings |= mask;
            board.white_pieces |= mask;
            board.utilized_squares |= mask;
            break;
        default:
            break;
        }
        x++;
    }
    std::cout << "\n\n";
    std::cout << "pawns:       \t" << std::bitset<64>(board.pawns) << std::endl;
    std::cout << "knights:     \t" << std::bitset<64>(board.knights) << std::endl;
    std::cout << "bishops:     \t" << std::bitset<64>(board.bishops) << std::endl;
    std::cout << "rooks:       \t" << std::bitset<64>(board.rooks) << std::endl;
    std::cout << "queens:      \t" << std::bitset<64>(board.queens) << std::endl;
    std::cout << "kings:       \t" << std::bitset<64>(board.kings) << std::endl;
    std::cout << "white_pieces:\t" << std::bitset<64>(board.white_pieces) << std::endl;
    std::cout << "black_pieces:\t" << std::bitset<64>(board.black_pieces) << std::endl;
    std::cout << "\n\n";

    return board;
    ;
}