#!/usr/bin/python
# generates ranks

for i in range(8):
    result = 0
    for j in range(8):
        result += 2**(8 * i + j)
    print hex(result)
