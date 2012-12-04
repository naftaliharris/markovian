#!/usr/bin/python
# control_params.py
# simple generation scheme to give squares value based on their circle. (Control the center)

scores = ["1000", "1500", "2000", "3000"]

for i in range(8):
    for j in range(8):
        n = max([abs(float(i) - 3.5), abs(float(j) - 3.5)])
        if n == 0.5:
            print scores[3] + ","
        elif n <= 1.5:
            print scores[2] + ","
        elif n <= 2.5:
            print scores[1] + ","
        else:
            print scores[0] + ","
