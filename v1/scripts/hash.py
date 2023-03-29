import sys
def hash_djb2(s, a, b):
	hash = a
	rand = b
	for x in s:
		hash = (( hash << rand) + hash) + ord(x)
		hash = hash & 0xFFFFFFFF
	return hash

hash = int(sys.argv[2])
rand = int(sys.argv[3])
h = hash_djb2(sys.argv[1], hash, rand)
print(hex(h))
