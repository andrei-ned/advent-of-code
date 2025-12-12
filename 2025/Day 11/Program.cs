var graph = new Dictionary<string, List<string>>();

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        string line = streamReader.ReadLine();
        var racks = line.Split(" ");
        var outputs = new List<string>();
        var device = racks[0].Substring(0,3);
        graph.Add(device, outputs);
        for (int i = 1; i < racks.Length; i++)
        {
            outputs.Add(racks[i]);
        }
    }
}

long numPaths = 0;
Pathfind("you");
Console.WriteLine($"Total paths from you to out: {numPaths}");

void Pathfind(string current)
{
    if (current == "out")
    {
        numPaths++;
        return;
    }
    foreach(var neighbor in graph[current])
    {
        Pathfind(neighbor);
    }
}