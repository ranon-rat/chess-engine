import chess
from typing import List
import random 
testing_fens = [
    {
        "fen": "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "quantity": [20, 400, 8902, 197281, 4865609],
        "name": "basic-board",
    },
    {
        "fen": "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0",
        "quantity": [48, 2039, 97862, 4085603, 193690690],
        "name": "second",
    },
    {
        "fen": "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        "quantity": [14, 191, 2812, 43238, 674624],
        "name": "third",
    },
    {
        "fen": "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "quantity": [6, 264, 9467, 422333, 15833292],
        "name": "fourth",
    },
    {
        "fen": "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        "quantity": [44, 1486, 62379, 2103487, 89941194],
        "name": "fifth",
    },
    {
        "fen": "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
        "quantity": [46, 2079, 89890, 3894594, 164075551],
        "name": "sixth",
    },
]


def eval_positions(board: chess.Board):
    new_positions = []
    for m in board.legal_moves:
        simulated_board = board.copy()
        simulated_board.push(m)
        new_positions.append(simulated_board)
    return new_positions


def execute_positions(initial_positions: List[chess.Board]):
    new_positions = []
    for b in initial_positions:
        new_positions += eval_positions(b)

    return new_positions


def check_fens(positions: List[chess.Board], name: str, depth: int):
    fens = [b.fen() for b in positions]
    with open(f"moves-test/{name}-{depth}.txt", "r") as file:
        lines =[ l.strip("\n") for l in file.readlines()]
         
        for i,line in enumerate(lines):
            compare_fen = line.strip()
            if compare_fen in fens:
                continue
            print(f"non valid fen: {compare_fen}  {fens[i]} at index {i} in {name}-{depth}.txt")


def TestFen(depth: int, fen: str, expected: List[float],name:str):
    positions = [chess.Board(fen)]
    epoch = 0
    print("-" * 50)
    print(f"Testing FEN: {fen} | Expected: {expected}\n | name: {name}")
    while epoch < depth:
        positions = execute_positions(positions)
        print(
            f"Depth: {epoch + 1} Result: {len( positions) } positions | passed: {"✅" if len(positions) == expected[epoch]else "⁉️"} expected: {expected[epoch]}"
        )
        epoch += 1
    check_fens(positions, name, depth)
    print("-" * 50)


if __name__ == "__main__":
    for test in testing_fens:
        print(f"Testing FEN: {test['fen']}")
        TestFen(3, test["fen"], test["quantity"], test["name"])
    print("SUP")
