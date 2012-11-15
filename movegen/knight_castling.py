#!/usr/bin/python
# generates knight moves that attack castling squares

print "// knight_castling.h"
print "// constants for where knights may attack castling squares"
print "// In the following, Knight_Castling[0] = WKING, Knight_Castling[1] = WQUEEN, Knight_Castling[2] = BKING, ...[3] = BQUEEN"
print ""
print "#define KC_WKING 0"
print "#define KC_WQUEEN 1"
print "#define KC_BKING 2"
print "#define KC_BQUEEN 3"
print ""
print "const uint64_t Knight_Castling[4] = {"
# the code below is wrong because it double counts, and += 2**(.) is not the same as bitwise and
# for i in [0, 7]:
# 	result = 0
# 	for j in [4, 5, 6]: # KINGSIDE
# 		n = i * 8 + j
# 		if (i + 1 <= 7) and (j + 2 <= 7):
# 			result += 2**(n + 10)
# 		if (i + 1 <= 7) and (j - 2 >= 0):
# 			result += 2**(n + 6)
# 		if (i - 1 >= 0) and (j + 2 <= 7):
# 			result += 2**(n - 6)
# 		if (i - 1 >= 0) and (j - 2 >= 0):
# 			result += 2**(n - 10)
# 
# 		if (i + 2 <= 7) and (j + 1 <= 7):
# 			result += 2**(n + 17)
# 		if (i + 2 <= 7) and (j - 1 >= 0):
# 			result += 2**(n + 15)
# 		if (i - 2 >= 0) and (j + 1 <= 7):
# 			result += 2**(n - 15)
# 		if (i - 2 >= 0) and (j - 1 >= 0):
# 			result += 2**(n - 17)
# 	print str(hex(result)) + ","
# 
# 	result = 0
# 	for j in [2, 3, 4]: #QUEENSIDE
# 		n = i * 8 + j
# 		if (i + 1 <= 7) and (j + 2 <= 7):
# 			result += 2**(n + 10)
# 		if (i + 1 <= 7) and (j - 2 >= 0):
# 			result += 2**(n + 6)
# 		if (i - 1 >= 0) and (j + 2 <= 7):
# 			result += 2**(n - 6)
# 		if (i - 1 >= 0) and (j - 2 >= 0):
# 			result += 2**(n - 10)
# 
# 		if (i + 2 <= 7) and (j + 1 <= 7):
# 			result += 2**(n + 17)
# 		if (i + 2 <= 7) and (j - 1 >= 0):
# 			result += 2**(n + 15)
# 		if (i - 2 >= 0) and (j + 1 <= 7):
# 			result += 2**(n - 15)
# 		if (i - 2 >= 0) and (j - 1 >= 0):
# 			result += 2**(n - 17)
# 	print str(hex(result)) + ","
# 
# print "};"
