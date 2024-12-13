def get_button_xy(line):
    return int(line.rstrip().split("+")[-2].split(",")[0]), int(line.rstrip().split("+")[-1])

# naive solution
def solve_machine(a_x, a_y, b_x, b_y, target_x, target_y):
    is_solvable = False
    min_token_cost = 1000
    for i in range(100):
        for j in range(100):
            x = a_x * i + b_x * j
            y = a_y * i + b_y * j
            if target_x == x and target_y == y:
                is_solvable = True
                token_cost = 3 * i + j
                if token_cost < min_token_cost:
                    min_token_cost = token_cost
    return min_token_cost if is_solvable else 0

# Use Cramer's rule to solve system of linear equations: https://en.wikipedia.org/wiki/Cramer%27s_rule
def solve_machine2(a_x, a_y, b_x, b_y, t_x, t_y):
    det_c = a_x * b_y - a_y * b_x
    det_x = t_x * b_y - t_y * b_x
    det_y = a_x * t_y - a_y * t_x
    #print(f"Determinants: {det_c},{det_x},{det_y}")
    a = det_x / det_c
    b = det_y / det_c
    #print(f"Solution: {a},{b} {a.is_integer()},{b.is_integer()}")
    if not a.is_integer():
        return 0
    if not a.is_integer():
        return 0
    return int(a * 3 + b)

with open("Day 13/input.txt") as file:
    line_count = 0
    sum_tokens = 0
    sum_tokens2 = 0
    a_x, a_y, b_x, b_y, target_x, target_y = (0,)*6
    for line in file:
        line_count += 1
        if line_count % 4 == 1:
            a_x, a_y = get_button_xy(line)
            print(f"Button A: {a_x},{a_y}")
        elif line_count % 4 == 2:
            b_x, b_y = get_button_xy(line)
            print(f"Button B: {b_x},{b_y}")
        elif line_count % 4 == 3:
            target_x = int(line.rstrip().split('=')[-2].split(',')[0])
            target_y = int(line.rstrip().split('=')[-1])
            print(f"Target: {target_x},{target_y}")
            sum_tokens += solve_machine(a_x, a_y, b_x, b_y, target_x, target_y)
            target_x += 10000000000000
            target_y += 10000000000000
            sum_tokens2 += solve_machine2(a_x, a_y, b_x, b_y, target_x, target_y)
    print(f"Total token count: {sum_tokens}")
    print(f"Total token count: {sum_tokens2} (part two)")

