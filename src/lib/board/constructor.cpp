#include <board.h++>
#include <iostream>
#include <bitset>
Board::Board()
{
    // m_possible_moves.empty();
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
    size_t i = 0;
    
    for (i = 0; i < fen.size(); i++)
    {
        char character = fen[i];
        if (character == ' ')
        {
            i++;
            break;
        }

        if (isdigit(character))
        {
            uint8_t value = (character - '0');
            std::cout << value << " ";
            x += value;
            continue;
        }
        
        if (character == '/')
        {
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
            break;
        case 'P':
            board.pawns |= mask;
            board.white_pieces |= mask;
            break;
        case 'n':
            board.knights |= mask;
            board.black_pieces |= mask;
            break;
        case 'N':
            board.knights |= mask;
            board.white_pieces |= mask;
            break;
        case 'b':
            board.bishops |= mask;
            board.black_pieces |= mask;
            break;
        case 'B':
            board.bishops |= mask;
            board.white_pieces |= mask;
            break;
        case 'r':
            board.rooks |= mask;
            board.black_pieces |= mask;
            break;
        case 'R':
            board.rooks |= mask;
            board.white_pieces |= mask;
            break;
        case 'q':
            board.queens |= mask;
            board.black_pieces |= mask;
            break;
        case 'Q':
            board.queens |= mask;
            board.white_pieces |= mask;
            break;
        case 'k':
            board.kings |= mask;
            board.black_pieces |= mask;
            break;
        case 'K':
            board.kings |= mask;
            board.white_pieces |= mask;
            break;
        default:
            break;
        }
        x++;
    }

    std::vector<std::string> tokens;
    std::string current_token = "";
    
    for (size_t j = i; j < fen.size(); j++)
    {
        if (fen[j] == ' ')// empty shit
        {
            if (!current_token.empty())
            {
                tokens.push_back(current_token);
                current_token = "";
            }
        }
        else
        {
            // so i just add new parts to the tokens :)
            current_token += fen[j];
        }
    }
    if (!current_token.empty())
    {
        tokens.push_back(current_token);
    }
    
    // which tunr
    if (tokens.size() >= 1)
    {
        if (tokens[0] == "w")
            board.white_to_move = true;
        else if (tokens[0] == "b")
            board.white_to_move = false;
    }
    
    if (tokens.size() >= 2)
    {
        // castling rights
        std::string castling = tokens[1];
      
        if (castling != "-")
        {
            for (char c : castling)
            {
                switch (c)
                {
                case 'K':
                    board.white_can_castle_kingside = true;
                    break;
                case 'Q':
                    board.white_can_castle_queenside = true;
                    break;
                case 'k':
                    board.black_can_castle_kingside = true;
                    break;
                case 'q':
                    board.black_can_castle_queenside = true;
                    break;
                }
            }
        }
    }
    
    if (tokens.size() >= 3)
    {
        // En passant
        std::string enpassant_str = tokens[2];
        if (enpassant_str != "-")
        {
            int ep_x = enpassant_str[0] - 'a';
            int ep_y = enpassant_str[1] - '1';
            board.enpassant = 1ULL << ((ep_y * 8) + ep_x);
        }
    }
    
    board.attacked_squares=getAttackedSquares(board);
    // Debug output
    std::cout << "\n\n";
    std::cout << "pawns:       \t" << std::bitset<64>(board.pawns) << std::endl;
    std::cout << "knights:     \t" << std::bitset<64>(board.knights) << std::endl;
    std::cout << "bishops:     \t" << std::bitset<64>(board.bishops) << std::endl;
    std::cout << "rooks:       \t" << std::bitset<64>(board.rooks) << std::endl;
    std::cout << "queens:      \t" << std::bitset<64>(board.queens) << std::endl;
    std::cout << "kings:       \t" << std::bitset<64>(board.kings) << std::endl;
    std::cout << "white_pieces:\t" << std::bitset<64>(board.white_pieces) << std::endl;
    std::cout << "black_pieces:\t" << std::bitset<64>(board.black_pieces) << std::endl;
    std::cout<<"\n\n";
    std::cout<< "white_to_move: " << (board.white_to_move ? "true" : "false") << std::endl;
    std::cout << "white_can_castle_kingside: " << (board.white_can_castle_kingside ? "true" : "false") << std::endl;
    std::cout << "white_can_castle_queenside: " << (board.white_can_castle_queenside ? "true" : "false") << std::endl;
    std::cout << "black_can_castle_kingside: " << (board.black_can_castle_kingside ? "true" : "false") << std::endl;
    std::cout << "black_can_castle_queenside: " << (board.black_can_castle_queenside ? "true" : "false") << std::endl;
    std::cout << "enpassant:   \t" << std::bitset<64>(board.enpassant) << std::endl;
    std::cout << "utilized_squares:\t" << std::bitset<64>(GetUtilizedSquares(board)) << std::endl;
    std::cout << "\n\n";

    return board;
}