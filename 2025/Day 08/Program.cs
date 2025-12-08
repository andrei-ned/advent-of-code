using System.Numerics;
const int NUM_CONNECTIONS = 1000;
const int NUM_CIRCUITS = 3;

List<Vector3> boxes = new List<Vector3>();

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        string line = streamReader.ReadLine();
        var coords = line.Split(",");
        boxes.Add(new Vector3(int.Parse(coords[0]), int.Parse(coords[1]), int.Parse(coords[2])));
    }
}

List<(int,int)> combos = new List<(int, int)>();
for (int i = 0; i < boxes.Count; i++)
{
    for (int j = i + 1; j < boxes.Count; j++)
    {
        combos.Add((i,j));
    }
}

combos.Sort((a,b) => 
    Vector3.DistanceSquared(boxes[a.Item1], boxes[a.Item2])
    .CompareTo
    (Vector3.DistanceSquared(boxes[b.Item1], boxes[b.Item2]))
);

HashSet<(int,int)> connections = new HashSet<(int, int)>();
for (int i = 0; i < NUM_CONNECTIONS; i++) // n shortest connections...
{
    //Console.WriteLine($"{boxes[combos[i].Item1]} - {boxes[combos[i].Item2]}");
    connections.Add(combos[i]);
}

HashSet<int> visitedPartOne = new HashSet<int>();

int Visit(int boxIndex, HashSet<int> visited)
{
    if (visited.Contains(boxIndex))
    {
        return 0;
    }
    //Console.Write(boxes[boxIndex]);
    int visitCount = 1;
    visited.Add(boxIndex);
    for (int i = 0; i < boxIndex; i++)
    {
        (int,int) combo = (i,boxIndex);
        if (connections.Contains(combo))
        {
            visitCount += Visit(i, visited);
        }
    }
    for (int i = boxIndex + 1; i < boxes.Count; i++)
    {
        (int,int) combo = (boxIndex,i);
        if (connections.Contains(combo))
        {
            visitCount += Visit(i, visited);
        }
    }
    return visitCount;
}

List<int> circuitLengths = new List<int>();
for (int i = 0; i < boxes.Count; i++)
{
    var c = Visit(i, visitedPartOne);
    if (c > 0)
    {
        //Console.WriteLine($"\ncircuit was len {c}");
        circuitLengths.Add(c);
    }
}

circuitLengths.Sort();
long product = 1;
for (int i = 1; i <= NUM_CIRCUITS; i++)
{
    product *= circuitLengths[circuitLengths.Count - i];
}
Console.WriteLine($"Product of sizes of {NUM_CIRCUITS} largest circuits = {product}");

// --- Part 2 ---
int idx = 0;
HashSet<int> visitedPartTwo = new HashSet<int>();
while (true)
{
    var combo = combos[idx];

    visitedPartTwo.Add(combo.Item1);
    visitedPartTwo.Add(combo.Item2);

    //Console.WriteLine($"{idx} connections and {visitedPartTwo.Count} boxes visited");

    if (visitedPartTwo.Count == boxes.Count)
    {
        Console.WriteLine($"Last two junction boxes: {boxes[combo.Item1]} - {boxes[combo.Item2]}");
        long answer = (long)boxes[combo.Item1].X * (long)boxes[combo.Item2].X;
        Console.WriteLine($"Their x coords multiply to {answer}"); 
        break;
    }

    idx++;
}