with open("Day 19/input.txt") as file:
    lines = [line.rstrip() for line in file]
    patterns = lines[0].split(', ')
    desired = lines[2:]

patterns.sort()
#print(patterns)

def bs_first_letter(target_letter):
    l = 0
    r = len(patterns) - 1
    while l != r:
        m = (l + r) // 2
        if patterns[m][0] < target_letter:
            l = m + 1
        elif patterns[m][0] > target_letter:
            r = m - 1

tried_so_far = set()

def try_build_design(design, built_so_far = ""):
    #print(design)
    #print(built_so_far)
    target_letter = design[len(built_so_far)]
    tried_so_far.add(built_so_far)
    #print(f"{target_letter}")
    # binary search for first letter
    for pattern in patterns:
        if pattern[0] != target_letter:
            continue
        new_build = built_so_far + pattern
        if new_build in tried_so_far:
            continue
        if design == new_build:
            # print(pattern)
            return True
        if design.startswith(new_build):
            if try_build_design(design, new_build):
                # print(pattern)
                return True
    return False

count = 0
for design in desired:
    tried_so_far = set()
    possible = try_build_design(design)
    print(f"{design} -> {possible}")
    if possible:
        count += 1

print(f"{count} possible designs")

