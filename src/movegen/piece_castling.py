#!/usr/bin/python
# piece castling.py
# generates piece (and king) impediments to castling

print "// piece_castling.h"
print "// constants for where pieces obstruct castling"
print "// same ordering as in pawn_castling.h"
print ""
print "const uint64_t Piece_Castling[4] = {"

# KINGSIDE
result = 0
for j in [5, 6]: 
	result += 2**(0*8 + j)

print str(hex(result)) + ","

# QUEENSIDE
result = 0
for j in [1, 2, 3]: 
	result += 2**(0*8 + j)

print str(hex(result)) + ","

# KINGSIDE
result = 0
for j in [5, 6]: 
	result += 2**(7*8 + j)

print str(hex(result)) + ","

# QUEENSIDE
result = 0
for j in [1, 2, 3]: 
	result += 2**(7*8 + j)

print str(hex(result)) + ","

print "};"
