from itertools import combinations

z_output = [0] * 100
wires = {}
operations = {}

with open("Day 24/input.txt") as file:
    for line in file:
        if ':' in line:
            line_split = line.split(":")
            wires[line_split[0]] = int(line_split[1])
        else:
            line_split = line.rstrip().split(' ')
            if len(line_split) == 1:
                continue
            operations[line_split[-1]] = line_split[0:3]

def do_operations(operations):
    ops_to_do = operations.copy()
    while (ops_to_do):
        to_delete = []
        for out_wire, op in ops_to_do.items():
            if op[0] in wires and op[2] in wires:
                match op[1]:
                    case 'AND':
                        wires[out_wire] = wires[op[0]] & wires[op[2]]
                    case 'XOR':
                        wires[out_wire] = wires[op[0]] ^ wires[op[2]]
                    case 'OR':
                        wires[out_wire] = wires[op[0]] | wires[op[2]]
                to_delete.append(out_wire)
        for w in to_delete:
            del ops_to_do[w]

# print(wires)
# print(operations)

def get_binary(wire_prefix):
    #print(wires)
    binary_out = ""
    for i in range(100):
        z = f"{wire_prefix}{str(i).zfill(2)}"
        if z in wires:
            binary_out = f"{wires[z]}{binary_out}"
    return binary_out

# --- Part 1
do_operations(operations)
print(f"Result of z for part 1: {int(get_binary('z'), 2)}")
# ---

# --- Part 2
# this was a mix of analysing a visualising the gates in a unity project and the code below
x = int(get_binary('x'),2)
y = int(get_binary('y'),2)
z_actual = get_binary('z')
z_expected = bin(x+y)
print(f"0b{z_actual}")
print(z_expected)

bad = set()
input_counts = {}
gate_input_counts = {} # gate[input_wire] = {'xor':xor_count, 'and':and_count, 'or':or_count}

def add_to_input_counts(in_wire, gate):
    if in_wire not in input_counts:
        input_counts[in_wire] = 0
    input_counts[in_wire] += 1
    if in_wire not in gate_input_counts:
        gate_input_counts[in_wire] = {}
    if gate not in gate_input_counts:
        gate_input_counts[in_wire][gate] = 0
    gate_input_counts[in_wire][gate] += 1
for out_wire, op in operations.items():
    add_to_input_counts(op[0], op[1])
    add_to_input_counts(op[2], op[1])

# print(operations)
# print(gate_input_counts)
for out_wire, op in operations.items():
    if 'z' in out_wire and op[1] != 'XOR':
        print(f"{out_wire} -> z not coming from XOR gate")
        bad.add(out_wire)
    elif op[1] == 'AND' and input_counts[out_wire] != 1:
        print(f"{out_wire} -> AND gate result goes into more than 1 input")
        bad.add(out_wire)
    elif op[1] == 'OR' and input_counts[out_wire] != 2:
        print(f"{out_wire} -> OR gate result goes into less than 2 inputs")
        bad.add(out_wire)
    elif op[1] == 'AND':
        if 'XOR' in gate_input_counts[out_wire] or 'AND' in gate_input_counts[out_wire]:
            print(f"{out_wire} -> AND gate result goes into gate other than OR")
            bad.add(out_wire)

for wire, gate_counts in gate_input_counts.items():
    if wire not in operations:
        continue
    gate = operations[wire][1] # the wire is an output of this gate
    #print(f"{wire} {gate} -> {gate_counts}")
    
    if gate == 'AND':
        if 'OR' not in gate_counts:
            print(f"{wire} -> AND gate result goes into gate other than OR")
            bad.add(wire)
    elif gate == 'XOR':
        if 'OR' in gate_counts:
            print(f"{wire} -> XOR gate result goes into OR")
            bad.add(wire)

bad.add('qbw')
bad = list(bad)
bad.sort()
print(','.join(bad))
