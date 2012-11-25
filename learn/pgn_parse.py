# pgn_parse.py
# tools for parsing pgns

from __future__ import division
import re
import random
import sys

class ChessGame:
    def __init__(self, lines):

        # Fill in the meta data
        self.meta_data = {}
        for line in lines:
            m = re.match(r'\[(?P<key>[^"]*) "(?P<value>[^"]*)"\]', line)
            if m:
                self.meta_data[m.group('key')] = m.group('value')

        # Record the moves
        self.moves = []
        for move in lines[-1].split():
            m = re.match(r'[a-h][1-8][a-h][1-8][QRBN]?', move)
            if m: self.moves.append(m.group(0).lower())

    """Return a markovian-friendly string"""
    def __str__(self):
        result = ""
        if self.meta_data["Result"] == '1-0':
            result += "white\n"
        elif self.meta_data["Result"] == '1/2-1/2':
            result += "draw\n"
        elif self.meta_data["Result"] == '0-1':
            result += "black\n"
        else:
            assert False

        assert self.moves

        result += str(len(self.moves)) + "\n"
        result += "\n".join(self.moves)
        result += "\nGAME OVER\n"

        return result

class ChessGameFile:
    def __init__(self, f):
        self.f = f
        self.num_games = 0

    def __iter__(self):
        return self

    def next(self):
        newlines = 0
        lines = []
        while newlines < 2:
            line = self.f.readline()
            if line.strip():
                lines.append(line)
            else:
                newlines += 1
        self.num_games += 1
        if lines:
            return ChessGame(lines)
        else:
            raise StopIteration

    def random_games(self, n, use_game=lambda x: True):
        games = []
        while len(games) < n:
            game = self.next()
            if use_game(game):
                games.append(game)
        total = n
        for game in self:
            if use_game(game):
                total += 1
                if random.random() < n / total:
                    games[random.randrange(n)] = game
        random.shuffle(games)
        return games

def main():
    with open(sys.argv[1], "r") as f:
        cgf = ChessGameFile(f)
        for game in cgf.random_games(5):
            print game

if __name__ == "__main__":
    main()
