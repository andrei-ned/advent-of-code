using Microsoft.VisualBasic;

var graph = new Dictionary<string, List<string>>();
// var backwardsGraph = new Dictionary<string, List<string>>();

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

        // backwardsGraph.Add(device, new List<string>());
    }
}

// foreach(var item in graph)
// {
//     foreach(var outputs in item.Value)
//     {
//         backwardsGraph[item.Key].Add(item.Key);
//     }
// }

//long numPathsOne = Pathfind("you", "out");

//PathfindPartOne("you");
//Console.WriteLine($"Total paths from you to out: {numPathsOne}");
// Console.WriteLine($"Test: {Pathfind("dac", "fft")}");
// Console.WriteLine($"Test: {Pathfind("fft", "dac")}");
// Console.WriteLine($"Test: {Pathfind("you", "out")}");

long numPathsTwo = 0;

//PathfindPartTwo("svr", false, false);
Console.WriteLine($"Total paths from svr to out, passing through dac and fft: {numPathsTwo}");

// void PathfindPartOne(string current)
// {
//     if (current == "out")
//     {
//         numPathsOne++;
//         return;
//     }
//     foreach(var neighbor in graph[current])
//     {
//         PathfindPartOne(neighbor);
//     }
// }

//--- Part two
var targets = new List<string>{"out", "fft", "dac"};
var distances = new Dictionary<string, Dictionary<string, long>>();
foreach (var item in graph)
{
    var dict = new Dictionary<string, long>();
    foreach (var neighbor in item.Value)
    {
        dict.Add(neighbor, 1);
    }
    distances[item.Key] = dict;
}

foreach (var device in new List<string>{"svr", "fft", "dac"})
{
    while (Compress(device))
    {
    }
    Console.WriteLine($"{device}: {string.Join(",", distances[device])}");
}

long svr_to_fft = distances["svr"]["fft"];
long fft_to_dac = distances["fft"]["dac"];
long dac_to_out = distances["dac"]["out"];
long answer = svr_to_fft * fft_to_dac * dac_to_out;
Console.WriteLine($"Paths from svr to out that pass through dac and fft: {answer}");
Console.WriteLine(answer);
Console.WriteLine(long.MaxValue);
// 2277318146121 is too low

bool Compress(string device)
{
    bool madeAdjustments = false;
    var toDo = new List<(string, long)>();
    var current = distances[device];
    foreach (var distDict in current)
    {
        if (targets.Contains(distDict.Key))
            continue;
        toDo.Add((distDict.Key, distDict.Value));
    }
    foreach (var item in toDo)
    {
        if (!distances.ContainsKey(item.Item1))
        {
            continue;
        }
        madeAdjustments = true;
        current.Remove(item.Item1);
        foreach (var dist in distances[item.Item1])
        {
            if (current.ContainsKey(dist.Key))
            {
                current[dist.Key] += item.Item2 * dist.Value;
            }
            else
            {
                current.Add(dist.Key, item.Item2 * dist.Value);
            }
        }
    }
    return madeAdjustments;
}

foreach (var item in distances)
{
    Console.WriteLine($"{item.Key}: {string.Join(",", item.Value)}");
}
//---

// var visited = new HashSet<string>();
// void PathfindPartTwo(string current, bool dac, bool fft)
// {
//     if (current == "out")
//     {
//         path.Add("out");
//         Console.WriteLine(string.Join(", ", path));
//         if (dac && fft)
//             numPathsTwo++;
//         path.Remove("out");
//         return;
//     }
//     if (current == "dac")
//         dac = true;
//     if (current == "fft")
//         fft = true;
//     path.Add(current);
//     Console.WriteLine(string.Join(", ", path));
//     foreach(var neighbor in graph[current])
//     {
//         PathfindPartTwo(neighbor, dac, fft);
//     }
//     path.Remove(current);
// }

// returns total paths from start to target
int Pathfind(string start, string target)
{
    var path = new List<string>();
    int sum = 0;
    PathfindInner(start);
    return sum;

    void PathfindInner(string current)
    {
        if (path.Contains(current))
        {
            Console.WriteLine("we've been here before, we looping");
            return;
        }
        path.Add(current);
        if (current == target)
        {
            sum++;
            Console.WriteLine(string.Join(", ", path));
            path.Remove(current);
            return;
        }
        if (current == "out")
        {
            Console.WriteLine(string.Join(", ", path));
            path.Remove(current);
            return;
        }
        foreach(var neighbor in graph[current])
        {
            PathfindInner(neighbor);
        }
        path.Remove(current);
    }
}