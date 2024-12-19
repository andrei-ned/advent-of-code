with open("Day 19/input.txt") as file:
    lines = [line.rstrip() for line in file]
    patterns = lines[0].split(', ')
    desired = lines[2:]

cache = {}
def try_build_design(design):
    if design in cache:
        return cache[design]
    ways_to_build = 0
    for pattern in patterns:
        if design == pattern:
            ways_to_build += 1
        elif design.startswith(pattern):
            ways_to_build += try_build_design(design.removeprefix(pattern))
    cache[design] = ways_to_build
    return ways_to_build

count_possible_designs = 0
count_ways_to_build = 0
for design in desired:
    possible = try_build_design(design)
    count_ways_to_build += possible
    print(f"{design} -> {possible}")
    if possible > 0:
        count_possible_designs += 1

print(f"{count_possible_designs} possible designs that can be built in a total of {count_ways_to_build} ways")

