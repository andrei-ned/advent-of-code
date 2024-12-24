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
        
#print(wires)
#print(operations)

while (operations):
    to_delete = []
    for out_wire, op in operations.items():
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
        del operations[w]

#print(wires)
binary_out = ""
for i in range(100):
    z = f"z{str(i).zfill(2)}"
    if z in wires:
        binary_out = f"{wires[z]}{binary_out}"
print(binary_out)
print(int(binary_out, 2))