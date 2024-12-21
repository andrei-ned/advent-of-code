import os
import time

# +---+---+---+
# | 7 | 8 | 9 |
# +---+---+---+
# | 4 | 5 | 6 |
# +---+---+---+
# | 1 | 2 | 3 |
# +---+---+---+
#     | 0 | A |
#     +---+---+
num_keypad = {
    '7':(0,0), '8':(1,0), '9':(2,0),
    '4':(0,1), '5':(1,1), '6':(2,1),
    '1':(0,2), '2':(1,2), '3':(2,2),
               '0':(1,3), 'A':(2,3),
}

#     +---+---+
#     | ^ | A |
# +---+---+---+
# | < | v | > |
# +---+---+---+
arrow_keypad = {
               '^':(1,0), 'A':(2,0),
    '<':(0,1), 'v':(1,1), '>':(2,1),
}

def solve_any(code, keypad):
    out = ""
    curr = 'A'
    for key in code:
        pos_curr = list(keypad[curr])
        pos_next = list(keypad[key])
        # print(f"from {curr} to {key}")
        x_diff = pos_next[0] - pos_curr[0]
        y_diff = pos_next[1] - pos_curr[1]
        while x_diff > 0:
            x_diff -= 1
            out += ">"
        while y_diff < 0:
            y_diff += 1
            out += "^"
        while y_diff > 0:
            y_diff -= 1
            out += "v"
        while x_diff < 0:
            x_diff += 1
            out += "<"
        out += "A"
        # print(key)
        # print(out)
        curr = key
    print(out)
    return out

def solve_directional(code):
    return solve_any(code, arrow_keypad)

def solve_numeric(code):
    key_codes = solve_any(code, num_keypad)
    for i in range(2):
        key_codes = solve_directional(key_codes)
    return key_codes

def debug_key(code):
    def update_pos(key, pos):
        match key:
            case '>':
                pos[0] += 1
            case '<':
                pos[0] -= 1
            case '^':
                pos[1] -= 1
            case 'v':
                pos[1] += 1
        return pos

    pos = [2,0]
    pos2 = [2,0]
    pos_num = [2,3]
    keypad_string = """    +---+---+
    | ^ | A |
+---+---+---+
| < | v | > |
+---+---+---+

"""
    numpad_string = """+---+---+---+
| 7 | 8 | 9 |
+---+---+---+
| 4 | 5 | 6 |
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
    | 0 | A |
    +---+---+

"""
    out = ""
    for key in code:
        os.system('cls')
        print(key)
        pos = update_pos(key,pos)
        hovered_key1 = [i for i in arrow_keypad if arrow_keypad[i]==tuple(pos)][0]
        print(keypad_string.replace(hovered_key1,f"\033[92m{hovered_key1}\033[0m"))
        if key == 'A':
            pos2 = update_pos(hovered_key1,pos2)
        hovered_key2 = [i for i in arrow_keypad if arrow_keypad[i]==tuple(pos2)][0]
        print(keypad_string.replace(hovered_key2,f"\033[92m{hovered_key2}\033[0m"))
        if key == 'A' and hovered_key1 == 'A':
            pos_num = update_pos(hovered_key2, pos_num)
        hovered_key3 = [i for i in num_keypad if num_keypad[i]==tuple(pos_num)][0]
        print(numpad_string.replace(hovered_key3,f"\033[92m{hovered_key3}\033[0m"))
        if (key == 'A' and hovered_key1 == 'A' and hovered_key2 == 'A'):
            out += hovered_key3
        print(out)
        time.sleep(.5)


with open("Day 21/input.txt") as file:
    lines = [line.rstrip() for line in file]

sum1 = 0
for line in lines:
    key_presses = solve_numeric(line)
    length = len(key_presses)
    num = int(line[:-1])
    print(f"{line}: {key_presses}")
    print(f"{length} * {num}")
    # if line == "379A":
    #     debug_key(key_presses)
    sum1 += length * num

print(f"Part 1: {sum1}")

#os.system('cls')
