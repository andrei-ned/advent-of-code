var shapes = new List<char[][]>();
var regions = new List<Region>();

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        string line = streamReader.ReadLine();
        if (line.Contains("x")) // tree
        {
            var region = new Region();
            var colonSplit = line.Split(":");
            var regionSize = colonSplit[0].Split("x");
            region.width = int.Parse(regionSize[0]);
            region.height = int.Parse(regionSize[1]);
            var counts = colonSplit[1].Substring(1);
            region.shapeCounts = counts.Split(" ").Select(x => int.Parse(x)).ToList();
            regions.Add(region);
        }
        else // shape
        {
            char[][] shape = new char[3][];
            shape[0] = streamReader.ReadLine().ToArray();
            shape[1] = streamReader.ReadLine().ToArray();
            shape[2] = streamReader.ReadLine().ToArray();
            shapes.Add(shape);
            streamReader.ReadLine(); // empty line
        }
    }
}

int sum = 0;
List<int> boxSizes = shapes.Select(x => x.Select(a => a.Count("#")).Sum()).ToList();
foreach (var region in regions)
{
    int totalPresentsSize = 0;
    for (int i = 0; i < region.shapeCounts.Count; i++)
    {
        totalPresentsSize += boxSizes[i] * region.shapeCounts[i];
    }
    int area = region.width * region.height;
    Console.WriteLine($"{totalPresentsSize} needs to fit in {area}");
    if (totalPresentsSize < area)
    {
        sum++;
    }
}
Console.WriteLine($"{sum} regions should fit presents");

struct Region
{
    public int width, height;
    public List<int> shapeCounts;
}