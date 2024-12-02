safeCount1 = 0
safeCount2 = 0

def isSafe(levels):
    isAscending = levels[1] - levels[0] > 0
    i = 1
    while i < len(levels):
        diff = abs(levels[i] - levels[i-1])
        isAsc = levels[i] - levels[i-1] > 0
        if (isAsc != isAscending) or diff < 1 or diff > 3:
            return False
        else:
            i += 1
    return True

with open("Day 02\input.txt") as file:
    for line in file:
        levels = line.rstrip().split(" ")
        levels = [int(lvl) for lvl in levels]

        safe = isSafe(levels)
        if (safe):
            safeCount1 += 1
            safeCount2 += 1
        else:
            for i in range(len(levels)):
                x = levels[:]
                del x[i]
                if (isSafe(x)):
                    safeCount2 += 1
                    break


print(f"Total safe count: {safeCount1}")
print(f"Total safe count with Problem Dampener: {safeCount2}")