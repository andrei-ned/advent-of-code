List<(long,long)> freshRanges = new List<(long, long)>();

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        var line = streamReader.ReadLine();
        //Console.WriteLine(line);
        if (line.IsWhiteSpace())
        {
            break;
        }
        var numbers = line.Split('-');
        freshRanges.Add((long.Parse(numbers[0]), long.Parse(numbers[1])));
    }

    long freshTotal = 0;
    while (!streamReader.EndOfStream)
    {
        var line = streamReader.ReadLine();
        //Console.WriteLine(line);
        long num = long.Parse(line);
        foreach(var range in freshRanges)
        {
            if (num >= range.Item1 && num <= range.Item2)
            {
                //Console.WriteLine(num);
                freshTotal++;
                break;
            }
        }
    }
    Console.WriteLine($"Total fresh ids (part one): {freshTotal}");
}

freshRanges.Sort((range1, range2) => range1.Item1.CompareTo(range2.Item1));

long previousHigh = 0;
long total = 0;
foreach(var range in freshRanges)
{
    long low = range.Item1;
    long high = range.Item2;
    if (low <= previousHigh)
    {
        low = previousHigh + 1;
    }
    previousHigh = Math.Max(previousHigh, high);
    // Console.WriteLine($"{range.Item1}-{range.Item2}");
    // Console.WriteLine($"{low}-{high}");
    // Console.WriteLine();
    if (low > high)
        continue;
    total += high-low+1;
}

Console.WriteLine($"Total fresh ids (part two): {total}");