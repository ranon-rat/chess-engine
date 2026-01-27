#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include "board-api.h++"
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <vector>

BoardAPI::FenParts BoardAPI::getFenParts(const std::string &str) {
  // si es white entonces es 1 si no es white entonces no es uno
  std::vector<std::string> parts;
  parts.reserve(6);
  std::string current_token = "";
  for (char c : str) {
    if (c == ' ') {
      if (!current_token.empty()) {
        parts.push_back(current_token);
        current_token = "";
      }
    } else {
      current_token += c;
    }
  }
  if (!current_token.empty()) {
    parts.push_back(current_token);
  }
  if (parts.size() < 6) {
    throw "FEN string is not valid, not enough parts";
  }
  return FenParts{
      .piece_placement = parts[0],
      .active_color = parts[1],
      .castling_availability = parts[2],
      .en_passant_target_square = parts[3],
      .halfmove_clock = (uint8_t)std::stoi(parts[4]),
      .fullmove_number = (uint16_t)std::stoi(parts[5]),
  };
}
void BoardAPI::buildBoardPiecePlacementFen(BitWiseBoard &board,
                                           const std::string &fen_pieces) {
  int8_t x = 0;
  int8_t y = 0;
  for (size_t i = 0; i < fen_pieces.size(); i++) {
    char character = fen_pieces[i];
    if (character == ' ') {
      i++;
      break;
    }

    if (isdigit(character)) {
      uint8_t value = (character - '0');
      x += value;
      continue;
    }

    if (character == '/') {
      x = 0;
      y++;

      continue;
    }

    uint64_t mask = 1ULL << ((y * 8) + x);

    switch (character) {
    case 'p':
      board.pawns |= mask;
      board.black_pieces |= mask;
      board.pawns_c[BLACK_TO_MOVE]++;
      break;
    case 'P':
      board.pawns |= mask;
      board.white_pieces |= mask;
      board.pawns_c[WHITE_TO_MOVE]++;
      break;
    case 'n':
      board.knights |= mask;
      board.black_pieces |= mask;
      board.knights_c[BLACK_TO_MOVE]++;
      break;
    case 'N':
      board.knights |= mask;
      board.white_pieces |= mask;
      board.knights_c[WHITE_TO_MOVE]++;
      break;
    case 'b':
      board.bishops |= mask;
      board.black_pieces |= mask;
      board.bishops_c[BLACK_TO_MOVE]++;
      break;
    case 'B':
      board.bishops |= mask;
      board.white_pieces |= mask;
      board.bishops_c[WHITE_TO_MOVE]++;
      break;
    case 'r':
      board.rooks |= mask;
      board.black_pieces |= mask;
      board.rooks_c[BLACK_TO_MOVE]++;
      break;
    case 'R':
      board.rooks |= mask;
      board.white_pieces |= mask;
      board.rooks_c[WHITE_TO_MOVE]++;
      break;
    case 'q':
      board.queens |= mask;
      board.black_pieces |= mask;
      board.queens_c[BLACK_TO_MOVE]++;
      break;
    case 'Q':
      board.queens |= mask;
      board.white_pieces |= mask;
      board.queens_c[WHITE_TO_MOVE]++;
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
}

void BoardAPI::selectColorFen(BitWiseBoard &board,
                              const std::string &fen_color) {
  board.white_to_move = fen_color == "w";
}
void BoardAPI::castlingRightsFen(BitWiseBoard &board,
                                 const std::string &fen_castling) {
  // castling rights

  if (fen_castling == "-") {
    return;
  }
  for (char c : fen_castling) {
    switch (c) {
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
void BoardAPI::enPassantFen(BitWiseBoard &board,
                            const std::string &fen_en_passant) {

  if (fen_en_passant != "-") {
    int ep_x = fen_en_passant[0] - 'a';
    int ep_y = '8' - fen_en_passant[1];
    board.enpassant = 1ULL << ((ep_y * 8) + ep_x);
  }
}
BitWiseBoard BoardAPI::BuildFromFEN(const std::string &fen) {
  BitWiseBoard board;
  FenParts parts = getFenParts(fen);
  buildBoardPiecePlacementFen(board, parts.piece_placement);
  selectColorFen(board, parts.active_color);
  castlingRightsFen(board, parts.castling_availability);
  enPassantFen(board, parts.en_passant_target_square);
  board.half_move = parts.halfmove_clock;
  board.complete_move = parts.fullmove_number;
  board.attacked_squares = getAttackedSquares(board);
  board.potenital_attacks = getPotentialAttacks(board);
  board.zobrist = GetZobrist(board);

  return board;
}
// rnbqkbnr/p2p3p/5pp1/1pp1p3/4P2P/2N5/PPPP1PP1/R1BQKBNR w Qkq - 0 6
void BoardAPI::fenFromBoardPieces(const BitWiseBoard &board,
                                  std::string &fen_pieces) {
  for (int8_t y = 0; y < 8; y++) {
    int since_no_piece = 0;
    for (int8_t x = 0; x < 8; x++) {
      TypePiece piece =
          GetPieceFromCoord(BoardCoordinates{.x = x, .y = y}, board);
      if (piece.piece != Pieces::NONE) {
        if (since_no_piece > 0) {
          fen_pieces += std::to_string(since_no_piece);
        }
        since_no_piece = 0;
      }

      switch (piece.piece) {
      case Pieces::PAWN:
        fen_pieces += piece.isWhite ? "P" : "p";
        break;
      case Pieces::KNIGHT:
        fen_pieces += piece.isWhite ? "N" : "n";
        break;
      case Pieces::BISHOP:
        fen_pieces += piece.isWhite ? "B" : "b";
        break;
      case Pieces::ROOK:
        fen_pieces += piece.isWhite ? "R" : "r";
        break;
      case Pieces::QUEEN:
        fen_pieces += piece.isWhite ? "Q" : "q";
        break;
      case Pieces::KING:
        fen_pieces += piece.isWhite ? "K" : "k";
        break;
      default:
        // Patrón de tablero de ajedrez para casillas vacías
        since_no_piece++;
        break;
      }
    }
    if (since_no_piece > 0) {
      fen_pieces += std::to_string(since_no_piece);
    }
    if (y < 7)
      fen_pieces += "/";
  }
}
void BoardAPI::fenFromColor(const BitWiseBoard &board, std::string &fen_color) {
  fen_color += board.white_to_move ? "w" : "b";
}
void BoardAPI::fenFromCastlingRights(const BitWiseBoard &board,
                                     std::string &fen_castling) {
  int can_castle = 0;
  if (board.white_can_castle_kingside) {
    fen_castling += "K";
    can_castle++;
  }
  if (board.white_can_castle_queenside) {
    fen_castling += "Q";
    can_castle++;
  }
  if (board.black_can_castle_kingside) {
    fen_castling += "k";
    can_castle++;
  }
  if (board.black_can_castle_queenside) {
    fen_castling += "q";
    can_castle++;
  }
  if (can_castle == 0) {
    fen_castling += "-";
  }
}
bool BoardAPI::thereIsPawnNearEnPassant(const BitWiseBoard &board,
                                        const BoardCoordinates &origin) {
  // if there is an enpassant then there is a piece in the position that its
  // next to it
  uint64_t friend_squares =
      board.white_to_move ? board.white_pieces : board.black_pieces;
  int8_t direction = board.white_to_move ? 1 : -1;
  int8_t e_p1 = origin.x + 1;
  int8_t e_p2 = origin.x - 1;
  int8_t next_y = origin.y + direction;
  if (e_p1 < 8) {
    uint64_t mask = 1ULL << (next_y * 8 + e_p1);
    if (board.pawns & mask & friend_squares) {
      MaxMovesArray moves = GetMoves(
          BoardCoordinates{
              .x = e_p1,
              .y = next_y,
          },
          board);
      for (const auto &move : moves) {
        if (move.x == origin.x && move.y == origin.y)
          return true;
      }
    }
  }
  if (e_p2 >= 0) {
    uint64_t mask = 1ULL << (next_y * 8 + e_p2);
    if (board.pawns & mask & friend_squares) {
      MaxMovesArray moves = GetMoves(
          BoardCoordinates{
              .x = e_p2,
              .y = next_y,
          },
          board);
      for (const auto &move : moves) {
        if (move.x == origin.x && move.y == origin.y)
          return true;
      }
    }
  }
  return false;
}

void BoardAPI::fenFromEnPassant(const BitWiseBoard &board,
                                std::string &fen_en_passant) {
  // lets first find the en passant
  for (uint8_t i = 0; i < 64; i++) {
    uint64_t mask = (1ULL) << (i);

    if (!(mask & board.enpassant))
      continue;

    int8_t x = i % 8;
    int8_t y = (i / 8); // okay so now i have to
    // NOW I HAVE TO CHECK THE NEXT THING
    if (!thereIsPawnNearEnPassant(board, BoardCoordinates{.x = x, .y = y}))
      continue;
    // then i should do sometyhing like uhhh
    fen_en_passant += static_cast<char>('a' + x);
    fen_en_passant += static_cast<char>('8' - y);
    return;
  }
  fen_en_passant += "-";
}
// rnbqkbnr/p2p3p/5pp1/1pp1p3/4P2P/2N5/PPPP1PP1/R1BQKBNR w Qkq - 0 6

std::string BoardAPI::GetFen(const BitWiseBoard &board) {
  FenParts output;
  fenFromBoardPieces(board, output.piece_placement);

  fenFromColor(board, output.active_color);
  fenFromCastlingRights(board, output.castling_availability);
  fenFromEnPassant(board, output.en_passant_target_square);
  output.halfmove_clock = board.half_move;
  output.fullmove_number = board.complete_move;
  return std::format("{} {} {} {} {} {}", output.piece_placement,
                     output.active_color, output.castling_availability,
                     output.en_passant_target_square, output.halfmove_clock,
                     output.fullmove_number);
};