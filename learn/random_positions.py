# random_positions.py
# a simple script to output random positions

import sys, random

from pgn_parse import *

def main():
    n = int(sys.argv[2])
    with open(sys.argv[1], "r") as infile:
        cgf = ChessGameFile(infile)
        for game in cgf.random_games(n, lambda game: len(game.moves) > 30):
            game.moves = game.moves[0:random.randrange(10, len(game.moves))]
            print game.pgn_str()

if __name__ == "__main__":
    main()
