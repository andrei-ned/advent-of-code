def read_equations():
    with open("Day 07\input.txt") as file:
        equations = []
        for line in file:
            eq = line.rstrip().split(" ")
            eq[0] = eq[0][:-1]
            eq = [int(no) for no in eq]
            equations.append(eq)

        return equations
    

sum = 0
equations = read_equations()
for eq in equations:
    result = eq[0]
    possibilities = [eq[1]]
    for i in range(2,len(eq)):
        possibilities_add = [no + eq[i] for no in possibilities]
        possibilities_mult = [no * eq[i] for no in possibilities]
        possibilities_concat = [int(str(no) + str(eq[i])) for no in possibilities] # part two, remove concat for part one
        possibilities = possibilities_add + possibilities_mult + possibilities_concat
    if result in possibilities:
        sum += result
    #print(possibilities)

print(f"Sum results: {sum}")