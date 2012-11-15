#!/usr/bin/python
# pawn castling.py
# generates pawn (and king) impediments to castling

print "// pawn_castling.h"
print "// constants for where pawns (and kings) may attack castling squares"
print "// In the following, Pawn_Castling[0] = WKING, Pawn_Castling[1] = WQUEEN, Pawn_Castling[2] = BKING, ...[3] = BQUEEN"
print ""
print "#define PC_WKING 0"
print "#define PC_WQUEEN 1"
print "#define PC_BKING 2"
print "#define PC_BQUEEN 3"
print ""
print "const uint64_t Pawn_Castling[4] = {"

# KINGSIDE
result = 0
for j in [3, 4, 5, 6, 7]: 
	result += 2**(1*8 + j)

print str(hex(result)) + ","

# QUEENSIDE
result = 0
for j in [1, 2, 3, 4, 5]: 
	result += 2**(1*8 + j)

print str(hex(result)) + ","

# KINGSIDE
result = 0
for j in [3, 4, 5, 6, 7]: 
	result += 2**(6*8 + j)

print str(hex(result)) + ","

# QUEENSIDE
result = 0
for j in [1, 2, 3, 4, 5]: 
	result += 2**(6*8 + j)

print str(hex(result)) + ","

print "};"
