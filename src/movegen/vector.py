#!/usr/bin/python
# for vectors moving out from a particular square, including the final rank, NOT including the square itself
# Example: vectorN[a4] = a5 | a6 | a7 | a8

print "// vector.h"
print "// vectors moving out from a particular square, including the final rank, NOT including the square itself"
print "// example: vectorN[a4] = a5 | a6 | a7 | a8"
print ""

print "const uint64_t vectorN[64] = {"
for i in range(8):
	for j in range(8):
		n = 2**(8 * i + j)
		result = 0
		for k in range(1,8):
			if i + k < 8:
				result += 2**(8 * (i+k) + (j))
		print str(hex(result)) + ","
print "};"
print ""

print "const uint64_t vectorS[64] = {"
for i in range(8):
	for j in range(8):
		n = 2**(8 * i + j)
		result = 0
		for k in range(1,8):
			if i - k >=0:
				result += 2**(8 * (i-k) + (j))
		print str(hex(result)) + ","
print "};"
print ""

print "const uint64_t vectorE[64] = {"
for i in range(8):
	for j in range(8):
		n = 2**(8 * i + j)
		result = 0
		for k in range(1,8):
			if j + k < 8:
				result += 2**(8 * (i) + (j+k))
		print str(hex(result)) + ","
print "};"
print ""

print "const uint64_t vectorW[64] = {"
for i in range(8):
	for j in range(8):
		n = 2**(8 * i + j)
		result = 0
		for k in range(1,8):
			if j - k >=0:
				result += 2**(8 * (i) + (j-k))
		print str(hex(result)) + ","
print "};"
print ""
