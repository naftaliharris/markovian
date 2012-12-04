#!/usr/bin/python
# for generating rook moves

for i in range(8):
    for j in range(8):
        n = 2**(8 * i + j)
        result = 0
        for k in range(1,8):
            if i + k < 8:
                result += 2**(8 * (i+k) + (j))
            if j - k >=0:
                result += 2**(8 * (i) + (j-k))
            if i - k >=0:
                result += 2**(8 * (i-k) + (j))
            if j + k < 8:
                result += 2**(8 * (i) + (j+k))
        print hex(result)

