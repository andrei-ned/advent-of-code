stone_counts = {}

with open("Day 11/input.txt") as file:
    for line in file:
        print(line)
        stones = [int(x) for x in line.rstrip().split(" ")]
        for stone in stones:
            stone_counts[stone] = 1

blinks = 75
for i in range(blinks):
    new_stone_counts = {}
    for stone, count in stone_counts.items():
        s = str(stone)
        if stone == 0:
            if 1 not in new_stone_counts:
                new_stone_counts[1] = 0
            new_stone_counts[1] += count
        elif len(s) % 2 == 0:
            first_half  = int(s[:len(s)//2])
            second_half = int(s[len(s)//2:])
            if first_half not in new_stone_counts:
                new_stone_counts[first_half] = 0
            new_stone_counts[first_half] += count
            if second_half not in new_stone_counts:
                new_stone_counts[second_half] = 0
            new_stone_counts[second_half] += count
        else:
            if stone * 2024 not in new_stone_counts:
                new_stone_counts[stone * 2024] = 0
            new_stone_counts[stone * 2024] +=   count
    stone_counts = new_stone_counts
    print(f"After {i+1} blinks: {sum(stone_counts.values())} stones")
    #print(stone_counts)
