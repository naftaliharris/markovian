#!/usr/bin/python
# generates knight moves

for i in range(8):
    for j in range(8):
        n = i * 8 + j
        result = 0
        if (i + 1 <= 7) and (j + 2 <= 7):
            result += 2**(n + 10)
        if (i + 1 <= 7) and (j - 2 >= 0):
            result += 2**(n + 6)
        if (i - 1 >= 0) and (j + 2 <= 7):
            result += 2**(n - 6)
        if (i - 1 >= 0) and (j - 2 >= 0):
            result += 2**(n - 10)

        if (i + 2 <= 7) and (j + 1 <= 7):
            result += 2**(n + 17)
        if (i + 2 <= 7) and (j - 1 >= 0):
            result += 2**(n + 15)
        if (i - 2 >= 0) and (j + 1 <= 7):
            result += 2**(n - 15)
        if (i - 2 >= 0) and (j - 1 >= 0):
            result += 2**(n - 17)
        print hex(result)
