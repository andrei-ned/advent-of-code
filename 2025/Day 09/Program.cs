var redTiles = new List<(long,long)>();

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        string line = streamReader.ReadLine();
        var coords = line.Split(",");
        redTiles.Add((long.Parse(coords[0]),long.Parse(coords[1])));
    }
}

long area((long,long) tile1, (long,long) tile2)
{
    return Math.Abs(tile1.Item1-tile2.Item1+1) * Math.Abs(tile1.Item2-tile2.Item2+1);
}

long max = -1;
for (int i = 0; i < redTiles.Count; i++)
{
    for (int j = 0; j < redTiles.Count; j++)
    {
        max = Math.Max(max, area(redTiles[i], redTiles[j]));
    }
}

Console.WriteLine($"Biggest area is {max}");
