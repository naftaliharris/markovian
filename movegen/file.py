#!/usr/bin/python
# generates files

for j in range(8):
	result = 0
	for i in range(8):
		result += 2**(8 * i + j)
	print hex(result)
