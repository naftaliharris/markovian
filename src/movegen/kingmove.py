#!/usr/bin/python

for i in range(8):
    for j in range(8):
        n = i * 8 + j
        result = 0
        if j > 0:
            result += 2**(n - 1)
            if i > 0:
                result += 2**(n - 9)
            if i < 7:
                result += 2**(n + 7)
        if j < 7:
            result += 2**(n + 1)
            if i > 0:
                result += 2**(n - 7)
            if i < 7:
                result += 2**(n + 9)
        if i > 0:
            result += 2**(n - 8)
        if i < 7:
            result += 2**(n + 8)
        print hex(result)
