#!/usr/bin/python
# pawn_attacks.py
# masks for where pawns give attacks

print "// pawn_attacks.h"
print "// constants for where pawns may attack"
print "// In the following, pawn_attacks[color][square] = spaces where pawns of color may attack square"
print ""
print "const uint64_t pawn_attacks[2][64] = {"

# white pawn attacks
print "{"
for i in range(8):
    for j in range(8):
        n = 8*i + j
        result = 0
        if i >= 2:
            if j >= 1:
                result += 2**(n - 9)
            if j <= 6:
                result += 2**(n - 7)
        print str(hex(result))+","
print "},"

# black pawn attacks
print "{"
for i in range(8):
    for j in range(8):
        n = 8*i + j
        result = 0
        if i <= 5:
            if j >= 1:
                result += 2**(n + 7)
            if j <= 6:
                result += 2**(n + 9)
        print str(hex(result))+","
print "},"

print "};"
