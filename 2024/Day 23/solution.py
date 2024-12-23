graph = {}
nodes_set = set()

with open("Day 23/input.txt") as file:
    for line in file:
        nodes = line.rstrip().split("-")
        for node in nodes:
            nodes_set.add(node)
            if node not in graph:
                graph[node] = []
        graph[nodes[0]].append(nodes[1])
        graph[nodes[1]].append(nodes[0])

# --- Part 1
lan_parties_with_t = 0
explored_set = set()
for node in nodes_set:
    for i in range(len(graph[node])):
        node2 = graph[node][i]
        if node2 in explored_set:
            continue
        for j in range(i+1, len(graph[node])):
            node3 = graph[node][j]
            if node2 == node3:
                continue
            if node3 in explored_set:
                continue
            if node2 in graph[node3]:
                lan_party = [node, node2, node3]
                if any(n[0] == 't' for n in lan_party):
                    lan_parties_with_t += 1
                    # print(lan_party)
    explored_set.add(node)
print(f"There are {lan_parties_with_t} sets of three inter-connected computers where one computer name starts with t")

# --- Part 2
max_lan_size = -1
def check_lan(node_list, lan_list, explored_s):
    global max_lan_size
    for node in node_list:
        if node in explored_set or node in explored_s:
            continue
        if node in lan_list:
            continue
        if not all(node in graph[n] for n in lan_list):
            continue
        lan_list.append(node)
        check_lan(node_list, lan_list, explored_s)
        if len(lan_list) > max_lan_size:
            max_lan_size = len(lan_list)
            print(f"Found bigger lan size {max_lan_size}: {','.join(x for x in sorted(lan_list))}")
        lan_list.remove(node)
        explored_s.add(node)

explored_set = set()
for node in nodes_set:
    lan = [node]
    check_lan(graph[node], lan, set())
    explored_set.add(node)
