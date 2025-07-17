#include "board.h++"
#include <random>

struct ZobristTable
{
    uint64_t table_pieces[12][64];
    uint64_t table_enpassant[16];
    uint64_t castle_queen_side_white = 0;
    uint64_t castle_king_side_white = 0;
    uint64_t castle_queen_side_black = 0;
    uint64_t castle_king_side_black = 0;
    uint64_t white_to_move = 0;
};
ZobristTable InitZobristTable()
{
    std::mt19937_64 rng(0xCAFEBABE); // Semilla fija
    ZobristTable zobrist_table;
    for (int p = 0; p < 12; ++p)
    {
        for (int sq = 0; sq < 64; ++sq)
        {
            zobrist_table.table_pieces[p][sq] = rng();
        }
    }
    for (int i = 0; i < 16; i++)
    {
        zobrist_table.table_enpassant[i] = rng();
    }

    zobrist_table.castle_queen_side_white = rng();
    zobrist_table.castle_king_side_white = rng();
    zobrist_table.castle_queen_side_black = rng();
    zobrist_table.castle_king_side_black = rng();
    zobrist_table.white_to_move = rng();
    return zobrist_table;
}
const ZobristTable zobrist_lookup = InitZobristTable();
uint64_t Board::GetZobrist(const BitWiseBoard &board)
{
    uint64_t final_zobrist = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            BoardCoordinates piece_coords = {
                .x = x,
                .y = y,
            };
            const TypePiece piece = GetPieceFromCoord(piece_coords, board);
            if (piece.piece == NONE)
                continue;
            size_t p_pos = piece.piece + piece.isWhite * 6;
            uint64_t zobrist_cell = zobrist_lookup.table_pieces[p_pos][y * 8 + x];
            final_zobrist ^= zobrist_cell;
        }
    }
    size_t index = 0;
    for (int y = 1; y < 6; y += 5)
    {
        for (int x = 0; x < 8; x++)
        {
            size_t index_value = index;
            uint64_t mask = 1ull << (y * 8 + x);
            index++;
            if (!(mask & board.enpassant))
                continue;
            uint64_t zobrist_cell = zobrist_lookup.table_enpassant[index_value];
            final_zobrist ^= zobrist_cell;
        }
    }
    if (board.black_can_castle_kingside)
        final_zobrist ^= zobrist_lookup.castle_king_side_black;
    if (board.white_can_castle_kingside)
        final_zobrist ^= zobrist_lookup.castle_king_side_white;
    if (board.black_can_castle_queenside)
        final_zobrist ^= zobrist_lookup.castle_queen_side_black;
    if (board.white_can_castle_queenside)
        final_zobrist ^= zobrist_lookup.castle_queen_side_white;
    if(board.white_to_move)
        final_zobrist^=zobrist_lookup.white_to_move;
    final_zobrist ^= board.no_capture_no_pawn;
    return final_zobrist;
}