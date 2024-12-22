import os
import time
import functools

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
    'x':(0,3), '0':(1,3), 'A':(2,3),
}

#     +---+---+
#     | ^ | A |
# +---+---+---+
# | < | v | > |
# +---+---+---+
arrow_keypad = {
    'x':(0,0), '^':(1,0), 'A':(2,0),
    '<':(0,1), 'v':(1,1), '>':(2,1),
}

@functools.cache
def solve_directions(origin, target, is_directional):
    keypad = arrow_keypad if is_directional else num_keypad
    pos_curr = list(keypad[origin])
    pos_next = list(keypad[target])
    moves = ""
    priorities = "<^v>"
    x_diff = pos_next[0] - pos_curr[0]
    y_diff = pos_next[1] - pos_curr[1]
    if keypad == num_keypad:
        priorities = "<^v>"
        if pos_next[0] == 0 and pos_curr[1] == 3: # avoid going into hole by going left in bottom row
            priorities = ">^v<"
        if pos_curr[0] == 0 and pos_next[1] == 3: # avoid going into hole by going down
            priorities = ">^v<"
    else:
        priorities = "<^v>"
        if pos_next[0] == 0: # avoid going into hole right to left
            priorities = "v<^>"
        if pos_curr[0] == 0 and pos_next[1] == 0: # avoid going into hole upwards
            priorities = "<>^v"
    for dir in priorities:
        match dir:
            case '>':
                while x_diff > 0:
                    x_diff -= 1
                    moves += ">"
            case '<':
                while x_diff < 0:
                    x_diff += 1
                    moves += "<"
            case '^':
                while y_diff < 0:
                    y_diff += 1
                    moves += "^"
            case 'v':
                while y_diff > 0:
                    y_diff -= 1
                    moves += "v"
    return moves + 'A'

@functools.cache
def solve_length(from_key, to_key, depth):
    is_numpad = from_key.isdigit() or to_key.isdigit()
    keys = solve_directions(from_key, to_key, not is_numpad)
    if depth == target_depth:
        return len(keys)
    sum = 0
    curr = 'A'
    for key in keys:
        sum += solve_length(curr, key, depth + 1)
        curr = key
    return sum

def solve_length_init(code):
    sum = 0
    curr = 'A'
    for key in code:
        sum += solve_length(curr, key, 0)
        curr = key
    return sum


# only for part one codes
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
        time.sleep(.75)

with open("Day 21/input.txt") as file:
    lines = [line.rstrip() for line in file]

target_depth = 25
sum = 0
for line in lines:
    length = solve_length_init(line)
    num = int(line[:-1])
    sum += length * num

print(f"Sum of complexities for {target_depth} robot directional keypads: {sum}")