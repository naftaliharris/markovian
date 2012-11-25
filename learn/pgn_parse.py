# pgn_parse.py
# tools for parsing pgns

import re

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

if __name__ == "__main__":
    main()
