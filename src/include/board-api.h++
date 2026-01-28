#ifndef BOARD_H
#define BOARD_H
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "BitWiseBoard.h++"
#include "BoardTypes.h++"
#include "PiecesAndMoves.h++"
#include <optional>

typedef std::array<std::vector<PiecesDireciton>, Pieces::PIECE_COUNT - 1>
    ArrayPieces;
typedef std::vector<Move> Movements;

ArrayPieces InitPossibleMoves();
enum class TypeGame {
  User = 0,
  Bot,
  Simulation,
};
class BoardAPI // this is the playing board :)
{
public:
  BoardAPI() {}
  enum TypeFilter {
    Legal = 0,
    Defendable,
    Line,
  };
  // this is just for the fen parsing part
  struct FenParts {
    std::string piece_placement;
    std::string active_color;
    std::string castling_availability;
    std::string en_passant_target_square;
    uint8_t halfmove_clock;
    uint16_t fullmove_number;
  };

public:
  // these are for interal usage
  // this will return the pieces positions, its made for the graphic interface
  std::array<TypePiece, 64>
  GetPieces(BitWiseBoard &board); // this will return the pieces in the board
  // this builds a new board
  BitWiseBoard BuildFromFEN(const std::string &fen);
  // this just generates a fen
  std::string GetFen(const BitWiseBoard &board);
  MaxMovesArray
  // this will get the moves that your piece can do
  GetMoves(BoardCoordinates piece, const BitWiseBoard &board,
           TypeFilter filter = Legal,
           std::optional<bool> is_white =
               std::nullopt); // esto deberia de retornarme un uint64_t con los
                              // movimientos legales de la pieza
  BitWiseBoard
  /*
    this will make a move, is similar to the eval board part but it doesnt
    handles promotion it also handles different types of "games"

    if the typeGame is user, it will check if the move is legal, so we cannot
    just cheat. if the typegame is bot then it will avoid doing that but it will
    still handle some important operations like:
      - the zobrist hash generator
      - the attack and potential line squares
    if the typegame is simulation it will not check if the move is legal and it
    will not handle any of the operations

    */
  MakeMove(BoardCoordinates from, BoardCoordinates to,
           const BitWiseBoard &board,
           TypeGame game = TypeGame::Bot); // i make reference to the board in
  // that specific square
  /*
    what this does is that it evals a move
    if the move seems to have the promotion part it ends up using it for the
    promotion. Is only made to be used by the bot, not the human
  */
  BitWiseBoard EvalBoard(Move move, const BitWiseBoard &board);
  // so this one, will be used to getting general information from the board :)
  TypePiece GetPieceFromCoord(BoardCoordinates from, const BitWiseBoard &board);

  /**
    this checks if the board is ready to promote.
    It only checks the lsat rows of each side, to know if you can promote or
  not.
  **/
  bool IsReadyToPromote(const BitWiseBoard &board);
  /*
    this will check if a pawn will promote, it uses 3 masks
    the pawn mask, the friendly mask, the pre line mask and the last line mask

    with this we can know if we can promote or not, its used in the legal moves
    generator
  */
  bool CanPromote(const BoardCoordinates &from, const BoardCoordinates &to,
                  const BitWiseBoard &board);
  /*
  this will handle the promotion part, the from part is which piece you want to
  promote.

  */
  BitWiseBoard Promotion(BoardCoordinates from, const BitWiseBoard &board,
                         Pieces new_piece); // so you select the new piece :)
  // you will get where is the promotion square, in case you need it
  std::optional<BoardCoordinates> WhereIsPromotion(const BitWiseBoard &board);
  // this will return if you are on checkmate, draw, or if you can continue
  GameStates CheckBoardState(const BitWiseBoard &board);
  // this will return a list of the legal moves, including the promotion part.
  Movements GetLegalMoves(const BitWiseBoard &board);

public:
  // obviously this is information is useful for knowing what its happening here
  // :)
  // the name says it, it just tells you if the piece is an enemy
  bool EnemySquares(BoardCoordinates from, const BitWiseBoard &board,
                    bool is_white);
  // the name says it, it just tells you if the piece is a friend
  bool FriendSquares(BoardCoordinates from, const BitWiseBoard &board,
                     bool is_white);
  // this only check if the square is occupied
  bool OcuppiedSquares(BoardCoordinates from, const BitWiseBoard &board);
  // this will return you a mask of the occupied squares
  uint64_t GetOccupiedSquares(const BitWiseBoard &board);
  // this returns if the king is being checked or not after making a move. its
  // useful :D
  bool IsChecked(BoardCoordinates from, BoardCoordinates to,
                 const BitWiseBoard &board, bool from_white);
  // this returns the zobrist, it will be later used
  uint64_t GetZobrist(const BitWiseBoard &board);

private:
  // FEN BS
  FenParts getFenParts(const std::string &str);
  void buildBoardPiecePlacementFen(BitWiseBoard &board,
                                   const std::string &fen_pieces);
  void selectColorFen(BitWiseBoard &board, const std::string &fen_color);
  void castlingRightsFen(BitWiseBoard &board, const std::string &fen_castling);
  void enPassantFen(BitWiseBoard &board, const std::string &fen_en_passant);
  // now the reverse operation

  void fenFromBoardPieces(const BitWiseBoard &board, std::string &fen_pieces);
  void fenFromColor(const BitWiseBoard &board, std::string &fen_color);
  void fenFromCastlingRights(const BitWiseBoard &board,
                             std::string &fen_castling);
  void fenFromEnPassant(const BitWiseBoard &board, std::string &fen_en_passant);
  bool thereIsPawnNearEnPassant(const BitWiseBoard &board,
                                const BoardCoordinates &origin);

private:
  // this is for the rook, bishop, and queen
  void lineMoves(Pieces piece, BoardCoordinates origin,
                 const BitWiseBoard &board, MaxMovesArray &moves, bool is_white,
                 TypeFilter filter = Legal);
  // king, knight
  void oneLineMoves(Pieces piece, BoardCoordinates origin,
                    const BitWiseBoard &board, MaxMovesArray &movesbool,
                    bool is_white, TypeFilter filter = Legal);
  // only for the pawn
  void pawnMoves(BoardCoordinates origin, const BitWiseBoard &board,
                 MaxMovesArray &moves, bool is_white,
                 TypeFilter filter = TypeFilter::Legal);
  // Castling
  void castlingMoves(BoardCoordinates origin, const BitWiseBoard &board,
                     MaxMovesArray &moves, bool is_white, uint64_t attack_mask,
                     TypeFilter filter);

private: //  simulation shit
  uint64_t getAttackedSquares(const BitWiseBoard &board,
                              std::optional<bool> is_white = std::nullopt);
  uint64_t getPotentialAttacks(const BitWiseBoard &board,
                               std::optional<bool> is_white = std::nullopt);

  bool movementIsLegal(const BoardCoordinates &from, const BoardCoordinates &to,
                       const BitWiseBoard &board);

private:
  void moveRook(BoardCoordinates from, BitWiseBoard &new_board,
                const BitWiseBoard &board, uint64_t initial_mask,
                uint64_t target_mask);
  void movePawn(BoardCoordinates from, BoardCoordinates to,
                BitWiseBoard &new_board, uint64_t initial_mask,
                uint64_t target_mask, int8_t direction);
  void moveKing(BoardCoordinates from, BoardCoordinates to,
                BitWiseBoard &new_board, const BitWiseBoard &board,
                uint64_t initial_mask, uint64_t target_mask);
  void eatPawnEnPassant(BoardCoordinates from, BoardCoordinates to,
                        BitWiseBoard &new_board, const BitWiseBoard &board,
                        int8_t direction);
  void eatRook(BoardCoordinates to, uint64_t target_mask,
               BitWiseBoard &new_board, const BitWiseBoard &board);

private:
  void getLegalFromPawn(const BitWiseBoard &board, Movements &output,
                        const BoardCoordinates &from, MaxMovesArray &to_moves);
  void getLegalFromRest(Movements &output, const BoardCoordinates &from,
                        MaxMovesArray &to_moves);

private:
  const ArrayPieces m_possible_moves =
      InitPossibleMoves(); // this is the vector of possible moves
};

#endif