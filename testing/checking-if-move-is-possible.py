
import chess
import os

files=[
    "basic-board-3.txt",
    "second-3.txt",
    "third-3.txt",
    "fourth-3.txt",
    "fifth-3.txt",
    "sixth-3.txt",
]


def get_boards():

    for file in files:

        with open(f"moves-test/{file}", "r", encoding="utf-8") as f:
            lines = f.readlines()
            # new fen, from,to, previous fen
            bad_moves = []                
            for line in lines:
                #8/8/2pp4/1P5r/KR3p1k/8/4P1P1/8 w - - 0 2,c7,c6,8/2p5/3p4/1P5r/KR3p1k/8/4P1P1/8 b - - 1 1
                [fen, from_square, to_square, previous_fen] = line.strip().split(",")
                board = chess.Board(previous_fen)
                from_sq = chess.parse_square(from_square)
                to_sq = chess.parse_square(to_square)
                move = chess.Move(from_sq, to_sq)
                if move  not in board.legal_moves:
                    bad_moves.append(line.strip())
            if bad_moves:
                with open(f"moves-test/non-valid/bad-moves-{file}", "w", encoding="utf-8") as bad_file:
                    for move in bad_moves:
                        bad_file.write(f"{move}\n")
                print(f"Bad moves in {file}:", len(bad_moves))
                
if __name__ == "__main__":
    get_boards()
    print("Finished checking moves")