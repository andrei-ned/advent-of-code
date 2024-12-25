locks = []
keys = []

def add_pattern(pattern):
    if pattern[0] == '#####':
        locks.append(pattern)
    else:
        keys.append(pattern)

with open("Day 25/input.txt") as file:
    current_pattern = []
    for line in file:
        if line[0] == "\n":
            add_pattern(current_pattern)
            current_pattern = []
        else:
            current_pattern.append(line.rstrip())
    add_pattern(current_pattern)

def count_heights(patterns, result_array):
    for pattern in patterns:
        this_nums = []
        for x in range(len(pattern[0])):
            height = -1
            for y in range(0,len(pattern)):
                if pattern[y][x] == '#':
                    height += 1
            this_nums.append(height)
        result_array.append(this_nums)

lock_nums = []
key_nums = []
count_heights(locks, lock_nums)
count_heights(keys,key_nums)

match_count = 0
for key in key_nums:
    for lock in lock_nums:
        match = True
        for i in range(len(key)):
            if key[i] + lock[i] > 5:
                match = False
                break
        if match:
            match_count += 1

print(f"{match_count} matches")
