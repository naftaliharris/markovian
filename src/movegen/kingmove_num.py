#!/usr/bin/python
# generates how many places a king may move to

print "// kingmove_num.h"
print "// the number of places a king at pos may move to"
print ""
print "const unsigned char kingmove_num[64] = {"
for i in range(8):
    for j in range(8):
        result = 0
        if j > 0:
            result += 1
            if i > 0:
                result += 1
            if i < 7:
                result += 1
        if j < 7:
            result += 1
            if i > 0:
                result += 1
            if i < 7:
                result += 1
        if i > 0:
            result += 1
        if i < 7:
            result += 1
        print str(result) + ","
print "};"
